#include "UploadAccessor.hpp"
#include "fwd.hpp"

namespace svh::video::components::http::get::upload {

UploadAccessor::UploadAccessor(const userver::components::ComponentConfig &cfg,
                               const userver::components::ComponentContext &ctx)
    : userver::server::handlers::HttpHandlerBase(cfg, ctx) {}

std::string UploadAccessor::HandleRequestThrow(
    const userver::server::http::HttpRequest &request,
    [[maybe_unused]] userver::server::request::RequestContext &context) const {
  EnableCORS(request);

  return std::string();
}

void UploadAccessor::EnableCORS(
    const userver::server::http::HttpRequest &request) const {
  using namespace std::string_view_literals;
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Origin"sv, "*");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Methods"sv,
                                      "OPTIONS, POST");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Headers"sv,
                                      "X-PINGOTHER, Content-Type");
}

void Append(userver::components::ComponentList &component_list) {
  component_list.Append<UploadAccessor>();
}

} // namespace svh::video::components::http::get::upload