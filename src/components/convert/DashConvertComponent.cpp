#include "DashConvertComponent.hpp"
#include "fwd.hpp"

#include <userver/components/component.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace svh::video::components::convert {

DashConvertComponent::DashConvertComponent(
    const userver::components::ComponentConfig& cfg,
    const userver::components::ComponentContext& ctx)
    : userver::components::LoggableComponentBase(cfg, ctx),
      convert_task_processor_(ctx.GetTaskProcessor(
          cfg["convert-task-processor"].As<std::string>())){};

userver::yaml_config::Schema DashConvertComponent::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
additionalProperties: false
description: component-converte
properties:
    convert-task-processor:
        type: string
        description : name of task processor to video chunks processing
  )");
}

void Append(userver::components::ComponentList& component_list) {
  component_list.Append<DashConvertComponent>();
}

}  // namespace svh::video::components::convert