#include "Uploader.hpp"
#include "fwd.hpp"

#include <logic/upload/controller/impl/UploadControllerImpl.hpp>
#include <userver/utils/async.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace svh::video::components::upload {

Uploader::Uploader(const userver::components::ComponentConfig& cfg,
                   const userver::components::ComponentContext& ctx)
    : userver::components::LoggableComponentBase(cfg, ctx),
      fs_task_processor_(
          ctx.GetTaskProcessor(cfg["fs-task-processor"].As<std::string>())),
      upload_controller_ptr_(
          std::make_shared<logic::upload::controller::UploadController>(
              fs_task_processor_)) {}

userver::yaml_config::Schema Uploader::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
additionalProperties: false
description: user component UploadHandler
properties:
    fs-task-processor:
        type: string
        description : name of task processor to video chunks processing
)");
}

std::shared_ptr<logic::upload::controller::UploadController>
Uploader::GetUploadControllerPtr() const {
  return upload_controller_ptr_;
}

void Append(userver::components::ComponentList& components_list) {
  components_list.Append<Uploader>();
}

}  // namespace svh::video::components::upload