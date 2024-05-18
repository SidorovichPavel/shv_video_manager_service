#include "UploadAccessor.hpp"
#include "fwd.hpp"

#include <jwt-cpp/jwt.h>
#include <userver/storages/postgres/component.hpp>

namespace svh::video::components::http::get::upload {

UploadAccessor::UploadAccessor(const userver::components::ComponentConfig& cfg,
                               const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerJsonBase(cfg, ctx),
      pg_cluster_(ctx.FindComponent<userver::components::Postgres>(
                         "svh_video_service_db")
                      .GetCluster()) {}

userver::formats::json::Value UploadAccessor::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context) const {
  EnableCORS(request);

  userver::formats::json::ValueBuilder json_builder;
  using namespace std::string_view_literals;
  json_builder["message"] = "Ok";
  return json_builder.ExtractValue();
}

void UploadAccessor::EnableCORS(
    const userver::server::http::HttpRequest& request) const {
  using namespace std::string_view_literals;
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Origin"sv, "*");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Methods"sv,
                                      "OPTIONS, POST, GET");
}

void Append(userver::components::ComponentList& component_list) {
  component_list.Append<UploadAccessor>();
}

}  // namespace svh::video::components::http::get::upload