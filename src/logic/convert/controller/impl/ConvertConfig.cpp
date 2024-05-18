#include "ConvertConfig.hpp"

#include <string>
#include <string_view>

#include <fmt/core.h>
#include <fmt/format.h>
#include <userver/formats/json.hpp>
#include <userver/logging/log.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svh::video::logic::convert::controller::impl {

namespace {
/*constexpr std::string_view kBasicFFmpeg =
    "/usr/bin/ffmpeg -hide_banner -y -i {} "
    "-r 25 -c:v h264 -pix_fmt yuv420p -preset veryfast -profile:v main "
    "-keyint_min 250 -g 250 -sc_threshold 0 "
    "-c:a aac -b:a 128k -ac 2 -ar 48000 "
    "-map v:0 -filter:v:0 \"scale=-2:360\"  -b:v:0 800k  -maxrate:0 856k "
    "-bufsize:0 1200k "
    "-map v:0 -filter:v:1 \"scale=-2:432\"  -b:v:1 1400k -maxrate:1 1498k "
    "-bufsize:1 2100k "
    "-map v:0 -filter:v:2 \"scale=-2:540\"  -b:v:2 2000k -maxrate:2 2140k "
    "-bufsize:2 3500k "
    "-map v:0 -filter:v:3 \"scale=-2:720\"  -b:v:3 2800k -maxrate:3 2996k "
    "-bufsize:3 4200k "
    "-map v:0 -filter:v:4 \"scale=-2:1080\" -b:v:4 5000k -maxrate:4 5350k "
    "-bufsize:4 7500k "
    "-map 0:a "
    "-dash_segment_type mp4 "
    "-use_template 1 "
    "-use_timeline 0 "
    "-seg_duration 10 "
    "-adaptation_sets \"id=0,streams=v id=1,streams=a\" "
    "-f dash "
    "{}.mpd"; */

/*const auto ConfigJson = userver::formats::json::FromString(R"({
    "command": "/usr/bin/ffmpeg",
    "general_options": [
        "-hide_banner",
        "-y",
        "-i",
        "{}",
        "-r",
        "25",
        "-c:v",
        "h264",
        "-pix_fmt",
        "yuv420p",
        "-preset",
        "veryfast",
        "-profile:v",
        "main",
        "-keyint_min",
        "250",
        "-g",
        "250",
        "-sc_threshold",
        "0",
        "-c:a",
        "aac",
        "-b:a",
        "128k",
        "-ac",
        "2",
        "-ar",
        "48000"
    ],
    "video_configurations": [
        [
            "-filter:v:0",
            "scale=-2:360",
            "-b:v:0",
            "800k",
            "-maxrate:0",
            "856k",
            "-bufsize:0",
            "1200k"
        ],
        [
            "-filter:v:1",
            "scale=-2:432",
            "-b:v:1",
            "1400k",
            "-maxrate:1",
            "1498k",
            "-bufsize:1",
            "2100k"
        ],
        [
            "-filter:v:2",
            "scale=-2:540",
            "-b:v:2",
            "2000k",
            "-maxrate:2",
            "2140k",
            "-bufsize:2",
            "3500k"
        ],
        [
            "-filter:v:3",
            "scale=-2:720",
            "-b:v:3",
            "2800k",
            "-maxrate:3",
            "2996k",
            "-bufsize:3",
            "4200k"
        ],
        [
            "-filter:v:4",
            "scale=-2:1080",
            "-b:v:4",
            "5000k",
            "-maxrate:4",
            "5350k",
            "-bufsize:4",
            "7500k"
        ]
    ],
    "output_options": [
        "-map",
        "0:a",
        "-dash_segment_type",
        "mp4",
        "-use_template",
        "1",
        "-use_timeline",
        "0",
        "-seg_duration",
        "10",
        "-adaptation_sets",
        "id=0,streams=v",
        "id=1,streams=a",
        "-f",
        "dash",
        "{}.mpd"
    ]
})");*/
}  // namespace

ConvertConfig::ConvertConfig()
    : frame_rate_(default_frame_rate),
      video_codec_(default_video_codec),
      keyint_min_(default_keyint_min),
      iframe_gen_step_(default_iframe_gen_step),
      seg_duration_(default_seg_duration),
      input_arg_index_(0u),
      output_arg_index_(0u) {
  command_args_ =
      std::vector<std::string>{"-hide_banner",
                               "-y",
                               "-i",
                               "{}",
                               "-r",
                               "25",
                               "-c:v",
                               "h264",
                               "-pix_fmt",
                               "yuv420p",
                               "-preset",
                               "veryfast",
                               "-profile:v",
                               "main",
                               "-keyint_min",
                               "250",
                               "-g",
                               "250",
                               "-sc_threshold",
                               "0",
                               "-c:a",
                               "aac",
                               "-b:a",
                               "128k",
                               "-ac",
                               "2",
                               "-ar",
                               "48000",
                               "-map",
                               "v:0",
                               "-filter:v:0",
                               "scale=-2:360",
                               "-b:v:0",
                               "800k",
                               "-maxrate:0",
                               "856k",
                               "-bufsize:0",
                               "1200k",
                               "-map",
                               "v:0",
                               "-filter:v:1",
                               "scale=-2:432",
                               "-b:v:1",
                               "1400k",
                               "-maxrate:1",
                               "1498k",
                               "-bufsize:1",
                               "2100k",
                               "-map",
                               "v:0",
                               "-filter:v:2",
                               "scale=-2:540",
                               "-b:v:2",
                               "2000k",
                               "-maxrate:2",
                               "2140k",
                               "-bufsize:2",
                               "3500k",
                               "-map",
                               "v:0",
                               "-filter:v:3",
                               "scale=-2:720",
                               "-b:v:3",
                               "2800k",
                               "-maxrate:3",
                               "2996k",
                               "-bufsize:3",
                               "4200k",
                               "-map",
                               "v:0",
                               "-filter:v:4",
                               "scale=-2:1080",
                               "-b:v:4",
                               "5000k",
                               "-maxrate:4",
                               "5350k",
                               "-bufsize:4",
                               "7500k",
                               "-map",
                               "0:a",
                               "-init_seg_name",
                               "init-$RepresentationID$.$ext$",
                               "-media_seg_name",
                               "chunk-$RepresentationID$-$Number%05d$.$ext$",
                               "-dash_segment_type",
                               "mp4",
                               "-use_template",
                               "1",
                               "-use_timeline",
                               "0",
                               "-seg_duration",
                               "10",
                               "-adaptation_sets",
                               "id=0,streams=v id=1,streams=a",
                               "-f",
                               "dash",
                               "{}/manifest.mpd"};
}

std::pair<std::string, std::vector<std::string>> ConvertConfig::apply(
    std::string_view input, std::string_view output) {
  std::string command("/bin/ffmpeg");
  command_args_[3] = input;
  command_args_.back() = fmt::format("{}/manifest.mpd", output);
  return std::make_pair(std::move(command), command_args_);
}

// void ConvertConfig::set_video_codec(std::string_view video_codec) {
//   video_codec_ = video_codec;
// }

// void ConvertConfig::set_video_codec_preset(
//     std::string_view video_codec_preset) {}

}  // namespace svh::video::logic::convert::controller::impl