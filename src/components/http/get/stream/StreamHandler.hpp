#pragma once

#include <string_view>

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_response_body_stream_fwd.hpp>

namespace svh::video::components::http::get::stream {

class StreamHandler final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "stream-handler";

  StreamHandler(const userver::components::ComponentConfig& cfg,
                const userver::components::ComponentContext& ctx);

  void HandleStreamRequest(
      const userver::server::http::HttpRequest&,
      userver::server::request::RequestContext&,
      userver::server::http::ResponseBodyStream&) const override;

 private:
  std::size_t GetStreamId(std::string_view stream) const;
};

}  // namespace svh::video::components::http::get::stream