#include "DashConvertComponent.hpp"
#include "fwd.hpp"
#include <UploadComponent.hpp>

#include <controller/impl/ConvertControllerImpl.hpp>

#include <userver/components/component.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace svh::video::components::convert {

DashConvertComponent::DashConvertComponent(
    const userver::components::ComponentConfig &cfg,
    const userver::components::ComponentContext &ctx)
    : userver::components::ProcessStarter(cfg, ctx),
      convert_task_processor_(ctx.GetTaskProcessor(
          cfg["convert-task-processor"].As<std::string>())),
      convert_controller_ptr_(
          std::make_shared<logic::convert::controller::ConvertController>(
              convert_task_processor_, Get(),
              logic::convert::controller::ConfigBuilder{}.Make())),
      upload_controller_ptr_(ctx.FindComponent<upload::UploadComponent>()
                                 .GetUploadControllerPtr()) {
  upload_controller_ptr_->upload_callback(
      [this](std::string path, std::string filename) {
        convert_controller_ptr_->enqueue(std::move(path), std::move(filename));
      });
}

userver::yaml_config::Schema DashConvertComponent::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::ProcessStarter>(R"(
type: object
additionalProperties: false
description: component-converte
properties:
    convert-task-processor:
        type: string
        description : name of task processor to video chunks processing
  )");
}

void Append(userver::components::ComponentList &component_list) {
  component_list.Append<DashConvertComponent>();
}

} // namespace svh::video::components::convert