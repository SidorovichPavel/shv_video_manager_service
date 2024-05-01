#include "UploadController.hpp"

namespace svh::video::logic::upload::controller {

UploadController::UploadController(
    userver::engine::TaskProcessor& fs_task_processor)
    : task_processor_(fs_task_processor) {}

void UploadController::push_block(std::string uid, std::string file_name,
                                  std::size_t total_blocks_number,
                                  std::size_t block_idx,
                                  std::string_view value) {
  ExpirationFileBuilder* builder =
      get_file_builder(uid, file_name, total_blocks_number);

  builder->push_block(block_idx, value);
}

ExpirationFileBuilder* UploadController::get_file_builder(
    std::string uid, std::string file_name, std::size_t total_blocks) {
  auto builders_lock = builders_.Lock();
  if ((*builders_lock)[uid] == nullptr)
    (*builders_lock)[uid] =
        std::make_unique<ExpirationFileBuilder>(file_name, total_blocks);

  return (*builders_lock)[uid].get();
}

}  // namespace svh::video::logic::uploader::controller