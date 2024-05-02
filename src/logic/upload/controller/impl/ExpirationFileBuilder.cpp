#include "ExpirationFileBuilder.hpp"

namespace svh::video::logic::upload::controller::impl {

ExpirationFileBuilder::ExpirationFileBuilder(std::string file_name,
                                             std::size_t total_blocks_number,
                                             std::string workspace_dir,
                                             expiration::duration live_time)
    : file_builder_(file_name, total_blocks_number, workspace_dir),
      expire_controller_(live_time) {}

bool ExpirationFileBuilder::is_ready() { return file_builder_.is_ready(); }

std::string ExpirationFileBuilder::get_name() const {
  return file_builder_.get_name();
}

void ExpirationFileBuilder::push_block(std::size_t idx,
                                       std::string_view value) {
  file_builder_.push_block(idx, value);
}

void ExpirationFileBuilder::build(std::string dir) { file_builder_.build(dir); }

bool ExpirationFileBuilder::expired() const noexcept {
  return expire_controller_.expired();
}

void ExpirationFileBuilder::update() noexcept { expire_controller_.update(); }

}  // namespace svh::video::logic::upload::controller::impl