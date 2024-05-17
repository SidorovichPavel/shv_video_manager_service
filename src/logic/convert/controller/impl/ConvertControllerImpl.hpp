#pragma once

#include "ConvertConfig.hpp"

#include <atomic>
#include <filesystem>
#include <queue>
#include <string>
#include <string_view>

#include <userver/concurrent/variable.hpp>
#include <userver/engine/condition_variable.hpp>
#include <userver/engine/subprocess/process_starter.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/engine/task/task_with_result.hpp>

namespace svh::video::logic::convert::controller::impl {

class ConvertControllerImpl {
 public:
  constexpr static std::string_view default_work_dir = "/tmp/streams";

  ConvertControllerImpl(userver::engine::TaskProcessor&,
                        userver::engine::subprocess::ProcessStarter&,
                        std::unique_ptr<ConvertConfig>,
                        std::string_view = default_work_dir);
  ~ConvertControllerImpl();

  void enqueue(std::string, std::string);

 private:
  void convertion(std::string, std::string);

  constexpr static auto max_simultaneous_locks = 2u;
  constexpr static std::string_view convert_task_name = "convert-task";

  userver::engine::TaskProcessor& task_processor_;
  userver::engine::subprocess::ProcessStarter& process_starter_;
  std::filesystem::path work_dir_;
  std::unique_ptr<ConvertConfig> config_ptr_;

  userver::engine::TaskWithResult<void> convert_task_;  // mayby as array
  userver::concurrent::Variable<std::queue<std::pair<std::string, std::string>>>
      convert_queue_;
  userver::engine::ConditionVariable convert_condition_;
  std::atomic_bool stop_;
};

}  // namespace svh::video::logic::convert::controller::impl