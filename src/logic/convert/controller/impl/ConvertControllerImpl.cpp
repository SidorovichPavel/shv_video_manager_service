#include "ConvertControllerImpl.hpp"

#include <filesystem>

#include <fmt/core.h>
#include <fmt/format.h>
#include <userver/engine/async.hpp>
#include <userver/engine/semaphore.hpp>
#include <userver/engine/subprocess/process_starter.hpp>
#include <userver/engine/task/task_with_result.hpp>
#include <userver/utils/async.hpp>

namespace svh::video::logic::convert::controller::impl {

ConvertControllerImpl::ConvertControllerImpl(
    userver::engine::TaskProcessor& task_processor,
    userver::engine::subprocess::ProcessStarter& process_starter,
    std::unique_ptr<ConvertConfig> conf_ptr, std::string_view work_dir)
    : task_processor_(task_processor),
      process_starter_(process_starter),
      work_dir_(work_dir),
      config_ptr_(std::move(conf_ptr)),
      stop_(false) {
  if (!std::filesystem::exists(work_dir_))
    std::filesystem::create_directory(work_dir_);

  convert_task_ = userver::utils::Async(
      task_processor_, std::string(convert_task_name), [this] {
        for (;;) {
          std::string path, filename;
          {
            auto queue_lock = convert_queue_.UniqueLock();

            auto pred = [this, &queue_lock]() {
              return !queue_lock->empty() || stop_;
            };

            for (auto time_to_work = false; !time_to_work;) {
              time_to_work =
                  convert_condition_.Wait(queue_lock.GetLock(), pred);
            }

            if (stop_ && queue_lock->empty()) return;

            std::tie(path, filename) = std::move(queue_lock->front());
            queue_lock->pop();
          }

          /// TODO: implement as threadpool
          userver::engine::Semaphore sem(max_simultaneous_locks);
          {
            std::shared_lock lock(sem);
            convertion(path, filename);
          }
        }
      });
}

ConvertControllerImpl::~ConvertControllerImpl() {
  stop_ = true;
  convert_condition_.NotifyAll();
  convert_task_.Get();
}

void ConvertControllerImpl::enqueue(std::string path, std::string filename) {
  {
    auto queue_lock = convert_queue_.UniqueLock();
    queue_lock->emplace(std::move(path), std::move(filename));
  }
  convert_condition_.NotifyAll();
}

void ConvertControllerImpl::convertion(std::string path, std::string filename) {
  auto source = path + '/';
  source += filename;
  std::string filename_without_ext =
      filename.substr(0, filename.find_last_of('.'));
  std::filesystem::path outdir = work_dir_ / filename_without_ext;

  std::filesystem::create_directory(outdir);
  auto outfile = outdir / filename_without_ext;
  auto [command, args] = config_ptr_->apply(source, outfile.u8string());
  process_starter_.Exec(command, args);
}

}  // namespace svh::video::logic::convert::controller::impl