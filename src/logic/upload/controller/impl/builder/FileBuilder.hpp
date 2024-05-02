#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include <userver/concurrent/variable.hpp>
#include <userver/crypto/hash.hpp>
#include <userver/engine/shared_mutex.hpp>

namespace svh::video::logic::upload::controller::impl::builder {

extern const std::string k_default_build_dir;

class FileBuilder {
  friend bool operator<(const FileBuilder& rsh, const FileBuilder& lsh);

 public:
  FileBuilder(std::string file_name, std::size_t total_blocks_number,
              std::string workspace_dir = k_default_build_dir);

  // check what all blocks received
  bool is_ready();
  // save block of data

  std::string get_name() const;

  void push_block(std::size_t idx, std::string_view value);

  // build result file in directory
  void build(std::string path = k_default_build_dir);

 private:
  // return new sha256 sum if block not exist or was updated
  std::optional<std::string> get_hash_if_need_write(std::size_t idx,
                                          std::string_view value);

  void write_block(std::size_t idx, std::string_view value);

  void update_block_hash(std::size_t idx, std::string hash);

  std::filesystem::path workspace_direction_;
  userver::concurrent::Variable<std::vector<std::string>,
                                userver::engine::SharedMutex>
      blocks_hashes_;
  std::size_t total_blocks_;
  std::string file_name_;
};

}  // namespace svh::video::logic::upload::controller::impl::builder