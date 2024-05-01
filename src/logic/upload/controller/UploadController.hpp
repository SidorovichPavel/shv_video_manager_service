#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <userver/compiler/select.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/utils/fast_pimpl.hpp>

namespace svh::video::logic::upload::controller {

namespace impl {
class UploadControllerImpl;
}

class UploadController {
 public:
  UploadController(userver::engine::TaskProcessor& fs_task_processor);

  void push_block(std::string uid, std::string file_name,
                  std::size_t total_blocks_number, std::size_t block_idx,
                  std::string_view value);

 private:
  static constexpr std::size_t kImplSize =
      userver::compiler::SelectSize().For64Bit(496).For32Bit(4);

  userver::utils::FastPimpl<impl::UploadControllerImpl, kImplSize, 8> impl_;
};

}  // namespace svh::video::logic::upload::controller