#include "UploadController.hpp"

#include "impl/UploadControllerImpl.hpp"

namespace svh::video::logic::upload::controller {

UploadController::UploadController(
    userver::engine::TaskProcessor& fs_task_processor)
    : impl_(fs_task_processor) {}

void UploadController::push_block(std::string uid, std::string file_name,
                                  std::size_t total_blocks_number,
                                  std::size_t block_idx,
                                  std::string_view value) {
  impl_->push_block(uid, file_name, total_blocks_number, block_idx, value);
}

}  // namespace svh::video::logic::upload::controller