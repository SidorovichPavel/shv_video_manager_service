#pragma once

#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/engine/async.hpp>
#include <userver/server/component.hpp>
#include <userver/yaml_config/yaml_config.hpp>

#include <logic/upload/controller/UploadController.hpp>

namespace svh::video::components::upload {
class Uploader final : public userver::components::LoggableComponentBase {
 public:
  constexpr static std::string_view kName = "upload-processor";

  static userver::yaml_config::Schema GetStaticConfigSchema();

  Uploader(const userver::components::ComponentConfig& cfg,
           const userver::components::ComponentContext& ctx);

  std::shared_ptr<logic::upload::controller::UploadController>
  GetUploadControllerPtr() const;

 private:
  userver::engine::TaskProcessor& fs_task_processor_;
  std::shared_ptr<logic::upload::controller::UploadController>
      upload_controller_ptr_;
};
}  // namespace svh::video::components::upload