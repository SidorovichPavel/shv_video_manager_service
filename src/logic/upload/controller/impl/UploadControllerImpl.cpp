#include "UploadControllerImpl.hpp"

#include <userver/utils/async.hpp>
#include <userver/utils/periodic_task.hpp>

namespace svh::video::logic::upload::controller::impl {

UploadControllerImpl::UploadControllerImpl(
    userver::engine::TaskProcessor& fs_task_processor)
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
  ExpirationFileBuilder* builder =
      get_file_builder(uid, file_name, total_blocks);

  builder->push_block(block_idx, value);

  builder->update();

  if (builder->is_ready()) {
  }
}

ExpirationFileBuilder* UploadControllerImpl::get_file_builder(
    std::string uid, std::string file_name, std::size_t total_blocks) {
  auto builders_lock = builders_.Lock();
  if ((*builders_lock)[uid] == nullptr)
    (*builders_lock)[uid] =
        std::make_unique<ExpirationFileBuilder>(file_name, total_blocks);

  return (*builders_lock)[uid].get();
}

}  // namespace svh::video::logic::upload::controller::impl