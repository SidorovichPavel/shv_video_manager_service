#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include <fmt/core.h>
#include <fmt/format.h>

namespace svh::video::logic::convert::controller::impl {

class ConvertConfig {
 public:
  ConvertConfig();
  std::string apply(std::string_view, std::string_view);

  void set_video_codec(std::string_view);
  void set_video_codec_preset(std::string_view);

  class VideoCodecPreset final : public std::optional<std::string> {
   public:
    using std::optional<std::string>::optional;
  };

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
  VideoCodecPreset video_codec_preset_;
  std::optional<std::uint16_t> keyint_min_;
  std::optional<std::uint16_t> iframe_gen_step_;
  std::uint16_t seg_duration_;
};

}  // namespace svh::video::logic::convert::controller::impl

template <>
struct fmt::formatter<svh::video::logic::convert::controller::impl::
                          ConvertConfig::VideoCodecPreset>
    : formatter<std::string> {
  auto format(const svh::video::logic::convert::controller::impl::
                  ConvertConfig::VideoCodecPreset& vcp,
              fmt::format_context& ctx) const;
};