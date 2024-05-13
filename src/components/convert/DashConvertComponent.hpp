#pragma once


#include <memory>
#include <string_view>

#include <controller/ConvertController.hpp>
#include <controller/UploadController.hpp>

#include <userver/components/process_starter.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/engine/async.hpp>
#include <userver/server/component.hpp>
#include <userver/yaml_config/yaml_config.hpp>

namespace svh::video::components::convert {

class DashConvertComponent final
    : public userver::components::ProcessStarter {
public:
  constexpr static std::string_view kName = "dash-converter";

  DashConvertComponent(const userver::components::ComponentConfig &cfg,
                       const userver::components::ComponentContext &ctx);

  static userver::yaml_config::Schema GetStaticConfigSchema();

private:
  userver::engine::TaskProcessor &convert_task_processor_;
  std::shared_ptr<logic::convert::controller::ConvertController>
      convert_controller_ptr_;
  std::shared_ptr<logic::upload::controller::UploadController>
      upload_controller_ptr_;
};

} // namespace svh::video::components::convert