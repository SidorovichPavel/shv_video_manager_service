#pragma once

#include <string_view>

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace svh::video::components::http::options {

class OptionsHandler final : public userver::server::handlers::HttpHandlerBase {
 public:
  constexpr static std::string_view kName = "options-handler";

  OptionsHandler(const userver::components::ComponentConfig& cfg,
                 const userver::components::ComponentContext& ctx);

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& request_context) const override;

 private:
};

}  // namespace svh::video::components::http::options