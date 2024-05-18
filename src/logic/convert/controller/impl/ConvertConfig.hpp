#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

namespace svh::video::logic::convert::controller::impl {

class ConvertConfig {
 public:
  ConvertConfig();

  std::pair<std::string, std::vector<std::string>> apply(std::string_view,
                                                         std::string_view);

  void set_video_codec(std::string_view);
  void set_video_codec_preset(std::string_view);

 private:
  constexpr static std::uint16_t default_frame_rate = 25;
  constexpr static std::string_view default_video_codec = "h264";
  constexpr static std::string_view default_video_codec_preset = "veryfast";
  constexpr static std::uint16_t default_seg_duration = 10;
  constexpr static std::uint16_t default_keyint_min =
      default_frame_rate * default_seg_duration;
  constexpr static std::uint16_t default_iframe_gen_step =
      default_frame_rate * default_seg_duration;

  std::uint16_t frame_rate_;
  std::string video_codec_;
  std::optional<std::uint16_t> keyint_min_;
  std::optional<std::uint16_t> iframe_gen_step_;
  std::uint16_t seg_duration_;

  std::size_t input_arg_index_;
  std::size_t output_arg_index_;
  std::vector<std::string> command_args_;
};

}  // namespace svh::video::logic::convert::controller::impl