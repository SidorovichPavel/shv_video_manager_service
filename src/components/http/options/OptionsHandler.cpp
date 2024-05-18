#include "OptionsHandler.hpp"
#include "fwd.hpp"

#include <userver/http/common_headers.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/component.hpp>

namespace svh::video::components::http::options {

OptionsHandler::OptionsHandler(const userver::components::ComponentConfig& cfg,
                               const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerBase(cfg, ctx) {}

std::string OptionsHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& context) const {
  using namespace std::string_view_literals;
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Origin"sv, "*");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Methods"sv,
                                      "OPTIONS, POST, GET");
  return std::string("Ok\n");
}

void Append(userver::components::ComponentList& component_list) {
  component_list.Append<OptionsHandler>();
}

}  // namespace svh::video::components::http::options