#include "ConvertConfig.hpp"

#include <iostream>
#include <string>
#include <string_view>

#include <fmt/core.h>
#include <fmt/format.h>
#include <userver/logging/log.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svh::video::logic::convert::controller::impl {

namespace {
constexpr std::string_view kBasicFFmpeg =
    R"(ffmpeg -hide_banner -y -i {} \
       -r {} -c:v {} -pix_fmt yuv420p {} -profile:v main \
       -keyint_min {} -g {} -sc_threshold 0 \
       -c:a aac -b:a 128k -ac 2 -ar 48000 \
       -map v:0 -filter:v:0 "scale=-2:360"  -b:v:0 800k  -maxrate:0 856k  -bufsize:0 1200k \
       -map v:0 -filter:v:1 "scale=-2:432"  -b:v:1 1400k -maxrate:1 1498k -bufsize:1 2100k \
       -map v:0 -filter:v:2 "scale=-2:540"  -b:v:2 2000k -maxrate:2 2140k -bufsize:2 3500k \
       -map v:0 -filter:v:3 "scale=-2:720"  -b:v:3 2800k -maxrate:3 2996k -bufsize:3 4200k \
       -map v:0 -filter:v:4 "scale=-2:1080" -b:v:4 5000k -maxrate:4 5350k -bufsize:4 7500k \
       -map 0:a \
       -init_seg_name "init-\$RepresentationID\$.\$ext\$" \
       -media_seg_name "chunk-\$RepresentationID\$-\$Number%05d\$.\$ext\$" \
       -dash_segment_type mp4 \
       -use_template 1 \
       -use_timeline 0 \
       -seg_duration {} \
       -adaptation_sets "id=0,streams=v id=1,streams=a" \
       -f dash \
       {}.mpd
)";
}  // namespace

ConvertConfig::ConvertConfig()
    : frame_rate_(default_frame_rate),
      video_codec_(default_video_codec),
      video_codec_preset_(default_video_codec_preset),
      keyint_min_(default_keyint_min),
      iframe_gen_step_(default_iframe_gen_step),
      seg_duration_(default_seg_duration) {
#ifndef NDEBUG
  std::cout << apply("config-debug-in", "config-debug-out");
#endif
}

std::string ConvertConfig::apply(std::string_view input,
                                 std::string_view output) {
  return fmt::format(kBasicFFmpeg, input, frame_rate_, video_codec_,
                     video_codec_preset_, *keyint_min_, *iframe_gen_step_,
                     seg_duration_, output);
}

void ConvertConfig::set_video_codec(std::string_view video_codec) {
  video_codec_ = video_codec;
  video_codec_preset_ = std::nullopt;
}

void ConvertConfig::set_video_codec_preset(
    std::string_view video_codec_preset) {
  video_codec_preset_ = VideoCodecPreset(video_codec_preset);
}

}  // namespace svh::video::logic::convert::controller::impl

auto fmt::formatter<svh::video::logic::convert::controller::impl::
                        ConvertConfig::VideoCodecPreset>::
    format(const svh::video::logic::convert::controller::impl::ConvertConfig::
               VideoCodecPreset& vcp,
           fmt::format_context& ctx) const {
  return fmt::format_to(ctx.out(), (vcp ? "-preset {}" : ""), vcp.value());
}