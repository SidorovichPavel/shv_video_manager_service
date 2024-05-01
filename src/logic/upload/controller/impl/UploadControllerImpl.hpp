#pragma once

#include "ExpirationFileBuilder.hpp"

#include <map>
#include <memory>
#include <string>

#include <userver/concurrent/variable.hpp>
#include <userver/engine/shared_mutex.hpp>
#include <userver/utils/async.hpp>

namespace svh::video::logic::upload::controller::impl {

class UploadControllerImpl {
 public:
  UploadControllerImpl(userver::engine::TaskProcessor& fs_task_processor);

  void push_block(std::string uid, std::string file_name,
                  std::size_t total_blocks_number, std::size_t block_idx,
                  std::string_view value);

 private:
  ExpirationFileBuilder* get_file_builder(std::string uid,
                                          std::string file_name,
                                          std::size_t total_blocks);

  userver::concurrent::Variable<
      std::map<std::string, std::unique_ptr<ExpirationFileBuilder>>,
      userver::engine::Mutex>
      builders_;
};
}  // namespace svh::video::logic::upload::controller::impl