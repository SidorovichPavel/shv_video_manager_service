#include "UploadAccessor.hpp"
#include "fwd.hpp"

#include <jwt-cpp/jwt.h>
#include <boost/uuid/uuid_generators.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/query.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

namespace svh::video::components::http::post::access {

const userver::storages::Query kVideoInsert(
    "INSERT INTO svh_video_schema.videos "
    "(created_at,\"owner\",title,descrpt,age_rating) "
    "VALUES($1,$2,$3,$4,$5) "
    "RETURNING id",
    userver::storages::Query::Name("kVideoInsert"),
    userver::storages::Query::LogMode::kFull);

UploadAccessor::UploadAccessor(const userver::components::ComponentConfig& cfg,
                               const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerJsonBase(cfg, ctx),
      pg_cluster_(ctx.FindComponent<userver::components::Postgres>(
                         "svh_video_service_db")
                      .GetCluster()) {}

userver::formats::json::Value UploadAccessor::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    [[maybe_unused]] userver::server::request::RequestContext& request_context)
    const {
  EnableCORS(request);

  LOG_DEBUG() << request_json;

  auto title = request_json["title"].As<std::string>();
  auto descrpt = request_json["descrpt"].As<std::string>("");
  auto age_rating = request_json["age_rating"].As<std::string>();

  userver::storages::postgres::TimePointTz tptz(
      std::chrono::system_clock::now());
  auto pg_result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, kVideoInsert, tptz,
      boost::uuids::random_generator{}(), title, descrpt, age_rating);

  userver::formats::json::ValueBuilder json_builder;

  if (pg_result.IsEmpty()) {
    request.GetHttpResponse().SetStatus(
        userver::server::http::HttpStatus::kInternalServerError);
    return json_builder.ExtractValue();
  }
  auto vid = pg_result.AsSingleRow<std::int64_t>();

  json_builder["token"]["vid"] = vid;
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