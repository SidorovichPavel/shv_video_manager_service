#pragma once

#include "builder/FileBuilder.hpp"
#include "expiration/ExpirationController.hpp"

namespace svh::video::logic::upload::controller::impl {

class ExpirationFileBuilder {
  friend struct EFBComparator;

public:
  ExpirationFileBuilder(
      std::string file_name, std::size_t total_blocks,
      std::string workspace_dir = builder::k_default_build_dir,
      expiration::duration live_time = expiration::k_default_expiration_time);

  ExpirationFileBuilder(const ExpirationFileBuilder &) = delete;
  ExpirationFileBuilder(ExpirationFileBuilder &&) = delete;
  ExpirationFileBuilder &operator=(const ExpirationFileBuilder &) = delete;
  ExpirationFileBuilder &operator=(ExpirationFileBuilder &&) = delete;

  bool is_ready() const;

  std::string get_name() const;

  void push_block(std::size_t idx, std::string_view value);

  void build(std::string dir = builder::k_default_build_dir);

  bool expired() const noexcept;

  void update() noexcept;

 private:
  builder::FileBuilder file_builder_;
  expiration::ExpirationController expire_controller_;
};

} // namespace svh::video::logic::upload::controller::impl