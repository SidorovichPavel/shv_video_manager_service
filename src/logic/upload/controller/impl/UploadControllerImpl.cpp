#include "UploadControllerImpl.hpp"

#include <userver/utils/async.hpp>
#include <userver/utils/periodic_task.hpp>

namespace svh::video::logic::upload::controller::impl {

UploadControllerImpl::UploadControllerImpl(
    userver::engine::TaskProcessor &fs_task_processor)
    : fs_task_processor_(fs_task_processor), gc_task_() {
  userver::utils::PeriodicTask::Settings periodic_task_settings(default_period);
  periodic_task_settings.task_processor = &fs_task_processor;
  gc_task_.Start(std::string(gc_task_name), periodic_task_settings,
                 [&] { this->gc_task(); });
}

UploadControllerImpl::~UploadControllerImpl() { gc_task_.Stop(); }

void UploadControllerImpl::push_block(std::string uid, std::string file_name,
                                      std::size_t total_blocks,
                                      std::size_t block_idx,
                                      std::string_view value) {
  async_push(uid, file_name, total_blocks, block_idx, value).Get();
}

void UploadControllerImpl::upload_callback(
    std::function<void(std::string, std::string)> callback) {
  callback_ = callback;
}

void UploadControllerImpl::gc_task() {
  /// TODO: Garbage Collercot implementation
}

userver::engine::TaskWithResult<void> UploadControllerImpl::async_push(
    std::string uid, std::string file_name, std::size_t total_blocks,
    std::size_t block_idx, std::string_view value) {
  const std::string task_name("fs-blocking-write-task");
  return userver::utils::Async(
      fs_task_processor_, task_name,
      [this](std::string uid, std::string file_name, std::size_t total_blocks,
             std::size_t block_idx, std::string_view value) {
        auto builder = get_file_builder(uid, file_name, total_blocks);

        builder->push_block(block_idx, value);

        builder->update();

        if (builder->is_ready()) {
          builder->build();
          delete_file_builder(uid);
          callback_(builder::k_default_build_dir, uid);
        }
      },
      uid, file_name, total_blocks, block_idx, value);
}

ExpirationFileBuilder *
UploadControllerImpl::get_file_builder(std::string uid, std::string file_name,
                                       std::size_t total_blocks) {
  /// TODO: fix locking time

  auto builders_lock = builders_.Lock();
  if ((*builders_lock)[uid] == nullptr)
    (*builders_lock)[uid] =
        std::make_unique<ExpirationFileBuilder>(file_name, total_blocks);

  return (*builders_lock)[uid].get();
}

void UploadControllerImpl::delete_file_builder(std::string uid) {
  std::unique_ptr<ExpirationFileBuilder> target;
  {
    auto builders_shared_lock = builders_.Lock();

    target = std::move((*builders_shared_lock)[uid]);
    std::ignore = builders_shared_lock->erase(uid);
  }
}

} // namespace svh::video::logic::upload::controller::impl