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

  UploadController(const UploadController&) = delete;
  UploadController(UploadController&&) = delete;

  UploadController& operator=(const UploadController&) = delete;
  UploadController& operator=(UploadController&&) = delete;

 private:
  static constexpr std::size_t kImplSize =
      userver::compiler::SelectSize().For64Bit(848u);
  static constexpr std::size_t kImplAlignment =
      userver::compiler::SelectSize().For64Bit(16u);

  userver::utils::FastPimpl<impl::UploadControllerImpl, kImplSize,
                            kImplAlignment>
      impl_;
};

}  // namespace svh::video::logic::upload::controller