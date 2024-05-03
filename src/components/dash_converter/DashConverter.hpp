#pragma once

#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/engine/async.hpp>
#include <userver/server/component.hpp>
#include <userver/yaml_config/yaml_config.hpp>

namespace svh::video::components::dash_converter {

class DashConverter final : public userver::components::LoggableComponentBase {
 public:
  constexpr static std::string_view kName = "dash-converter";

  DashConverter(const userver::components::ComponentConfig& cfg,
                const userver::components::ComponentContext& ctx);

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::engine::TaskProcessor& convert_task_processor_;
};

}  // namespace svh::video::components::dash_converter