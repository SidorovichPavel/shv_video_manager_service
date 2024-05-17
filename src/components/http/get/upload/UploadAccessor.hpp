#pragma once

#include <string_view>

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace svh::video::components::http::get::upload {

class UploadAccessor final : public userver::server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "upload-accessor";

  UploadAccessor(const userver::components::ComponentConfig &cfg,
                 const userver::components::ComponentContext &ctx);

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest &request,
      userver::server::request::RequestContext &context) const override;

private:
  void EnableCORS(const userver::server::http::HttpRequest &request) const;
};

} // namespace svh::video::components::http::get::upload