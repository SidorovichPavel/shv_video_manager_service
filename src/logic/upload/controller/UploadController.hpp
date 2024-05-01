#pragma once

#include "ExpirationFileBuilder.hpp"

#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>

#include <userver/concurrent/variable.hpp>
#include <userver/engine/shared_mutex.hpp>

namespace svh::video::logic::upload::controller {

class UploadController {
 public:
  UploadController(userver::engine::TaskProcessor& fs_task_processor);

  void push_block(std::string uid, std::string file_name,
                  std::size_t total_blocks_number, std::size_t block_idx,
                  std::string_view value);

 private:
  ExpirationFileBuilder* get_file_builder(std::string uid,
                                          std::string file_name,
                                          std::size_t total_blocks);

  std::optional<std::string> ttry_find_b64_cache(std::string_view str);

  void update_b64_cache(std::string_view str);

  userver::engine::TaskProcessor& task_processor_;
  userver::concurrent::Variable<std::map<std::string, std::string>,
                                userver::engine::SharedMutex>
      base64_cache_;

  userver::concurrent::Variable<
      std::map<std::string, std::unique_ptr<ExpirationFileBuilder>>,
      userver::engine::Mutex>
      builders_;
};

}  // namespace svh::video::logic::uploader::controller