#include "ConvertController.hpp"
#include "impl/ConvertConfig.hpp"
#include "impl/ConvertControllerImpl.hpp"

namespace svh::video::logic::convert::controller {

ConfigBuilder::ConfigBuilder()
    : config_(std::make_unique<impl::ConvertConfig>()) {}

std::unique_ptr<impl::ConvertConfig> ConfigBuilder::Make() {
  return std::move(config_);
}

ConfigBuilder &ConfigBuilder::SetVideoCodec(std::string_view video_codec) {
  config_->set_video_codec(video_codec);
  return *this;
}

ConfigBuilder &
ConfigBuilder::SetVideoCodecPreset(std::string_view video_codec_preset) {
  config_->set_video_codec_preset(video_codec_preset);
  return *this;
}

ConvertController::ConvertController(
    userver::engine::TaskProcessor &task_processor,
    std::unique_ptr<impl::ConvertConfig> conv_cfg)
    : impl_(task_processor, std::move(conv_cfg)) {}

ConvertController::~ConvertController() {}

void ConvertController::enqueue(std::string filename) {
  impl_->enqueue(filename);
}

} // namespace svh::video::logic::convert::controller