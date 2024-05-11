#include "ConvertControllerImpl.hpp"

#include <filesystem>

#include <userver/engine/async.hpp>
#include <userver/engine/semaphore.hpp>
#include <userver/engine/task/task_with_result.hpp>
#include <userver/utils/async.hpp>

namespace svh::video::logic::convert::controller::impl {

ConvertControllerImpl::ConvertControllerImpl(
    userver::engine::TaskProcessor &task_processor,
    std::unique_ptr<ConvertConfig> conf_ptr, std::string_view work_dir)
    : task_processor_(task_processor), work_dir_(work_dir),
      config_ptr_(std::move(conf_ptr)), stop_(false) {
  if (!std::filesystem::exists(work_dir_))
    std::filesystem::create_directory(work_dir_);

  convert_task_ = userver::utils::Async(
      task_processor_, std::string(convert_task_name), [this] {
        for (;;) {
          std::string filename;
          {
            auto queue_lock = convert_queue_.UniqueLock();

            auto pred = [this, &queue_lock]() {
              return !queue_lock->empty() || stop_;
            };

            for (auto time_to_work = false; !time_to_work;) {
              time_to_work =
                  convert_condition_.Wait(queue_lock.GetLock(), pred);
            }

            if (stop_ && queue_lock->empty())
              return;

            filename = std::move(queue_lock->front());
            queue_lock->pop();
          }

          /// TODO: implement as threadpool
          userver::engine::Semaphore sem(max_simultaneous_locks);
          {
            std::shared_lock lock(sem);
            convertion(filename);
          }
        }
      });
}

ConvertControllerImpl::~ConvertControllerImpl() {
  stop_ = true;
  convert_condition_.NotifyAll();
  convert_task_.Get();
}

void ConvertControllerImpl::enqueue(std::string filename) {
  {
    auto queue_lock = convert_queue_.UniqueLock();
    queue_lock->push(std::move(filename));
  }
  convert_condition_.NotifyAll();
}

void ConvertControllerImpl::convertion(std::string filename) {
  auto path = std::filesystem::path(filename);
}

} // namespace svh::video::logic::convert::controller::impl