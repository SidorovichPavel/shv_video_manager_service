#include "VideosTakeHandler.hpp"
#include "fwd.hpp"

#include <jwt-cpp/jwt.h>
#include <boost/uuid/uuid_generators.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/io/array_types.hpp>
#include <userver/storages/postgres/io/row_types.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include <userver/storages/query.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

namespace svh::video::components::http::get::videos {

const userver::storages::Query kSelectVideos(
    "SELECT * FROM svh_video_schema.videos",
    userver::storages::Query::Name("kSelectVideos"),
    userver::storages::Query::LogMode::kFull);

VideosTakeHandler::VideosTakeHandler(
    const userver::components::ComponentConfig& cfg,
    const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerJsonBase(cfg, ctx),
      pg_cluster_(ctx.FindComponent<userver::components::Postgres>(
                         "svh_video_service_db")
                      .GetCluster()) {}

userver::formats::json::Value VideosTakeHandler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    [[maybe_unused]] userver::server::request::RequestContext& request_context)
    const {
  EnableCORS(request);

  auto pg_result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, kSelectVideos);
  LOG_DEBUG() << pg_result.Size();

  if (pg_result.IsEmpty()) return userver::formats::json::MakeArray();
  userver::formats::json::ValueBuilder json_builder;
  for (auto&& row : pg_result) {
    userver::formats::json::ValueBuilder json_obj;
    json_obj["id"] = row["id"].As<std::int64_t>();
    json_obj["title"] = row["title"].As<std::string>();
    json_obj["description"] = row["descrpt"].As<std::string>();
    json_builder.PushBack(std::move(json_obj));
  }
  return json_builder.ExtractValue();
}

void VideosTakeHandler::EnableCORS(
    const userver::server::http::HttpRequest& request) const {
  using namespace std::string_view_literals;
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Origin"sv, "*");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Methods"sv,
                                      "OPTIONS, POST, GET");
}

void Append(userver::components::ComponentList& component_list) {
  component_list.Append<VideosTakeHandler>();
}

}  // namespace svh::video::components::http::get::videos
