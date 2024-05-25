#pragma once

#include "ExpirationFileBuilder.hpp"

#include <map>
#include <memory>
#include <string>

#include <userver/concurrent/variable.hpp>
#include <userver/engine/shared_mutex.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/utils/periodic_task.hpp>

namespace svh::video::logic::upload::controller::impl {

class UploadControllerImpl {
 public:
  UploadControllerImpl(userver::engine::TaskProcessor& fs_task_processor);

  ~UploadControllerImpl();

  void push_block(std::string uid,
                  std::size_t total_blocks_number, std::size_t block_idx,
                  std::string_view value);

  // callback must be immutable
  void upload_callback(std::function<void(std::string, std::string)> callback);

 private:
  constexpr static std::chrono::milliseconds default_period{
      std::chrono::seconds(15)};
  constexpr static std::string_view gc_task_name = "gc-task-name";

  userver::engine::TaskWithResult<void> async_push(std::string uid,
                                                   std::size_t total_blocks,
                                                   std::size_t block_idx,
                                                   std::string_view value);

  void gc_task();

  ExpirationFileBuilder* get_file_builder(std::string uid,
                                          std::size_t total_blocks);

  void delete_file_builder(std::string uid);

  userver::concurrent::Variable<
      std::map<std::string, std::unique_ptr<ExpirationFileBuilder>>,
      userver::engine::Mutex>
      builders_;
  userver::engine::TaskProcessor& fs_task_processor_;
  userver::utils::PeriodicTask gc_task_;

  std::function<void(std::string, std::string)> on_constructed_callback_;
};
}  // namespace svh::video::logic::upload::controller::impl