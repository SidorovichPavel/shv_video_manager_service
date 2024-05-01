#pragma once

#include <cstdint>
#include <string>

namespace svh::video::logic::upload::controller::impl {

class ExpirationFileBuilder;

class UploadControllerImplBase {
 public:
  UploadControllerImplBase() = default;

  virtual void push_block(std::string uid, std::string file_name,
                          std::size_t total_blocks_number,
                          std::size_t block_idx, std::string_view value) = 0;

  virtual ~UploadControllerImplBase() = default;
};

}  // namespace svh::video::logic::upload::controller::impl