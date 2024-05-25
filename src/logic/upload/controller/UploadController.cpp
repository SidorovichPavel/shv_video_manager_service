#include "UploadController.hpp"

#include "impl/UploadControllerImpl.hpp"

namespace svh::video::logic::upload::controller {

UploadController::UploadController(
    userver::engine::TaskProcessor& fs_task_processor)
    : impl_(fs_task_processor) {}

void UploadController::push_block(std::string uid,
                                  std::size_t total_blocks_number,
                                  std::size_t block_idx,
                                  std::string_view value) {
  impl_->push_block(uid, total_blocks_number, block_idx, value);
}

void UploadController::upload_callback(
    std::function<void(std::string, std::string)> callback) {
  impl_->upload_callback(callback);
}

}  // namespace svh::video::logic::upload::controller