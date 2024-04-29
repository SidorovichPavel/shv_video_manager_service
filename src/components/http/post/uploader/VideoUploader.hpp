#include <string_view>

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace svh::video::components::http::post::uploader {

class VideoUploader final : public userver::server::handlers::HttpHandlerBase {
 public:
  constexpr static std::string_view kName = "video-uploader";

  VideoUploader(const userver::components::ComponentConfig& cfg,
                const userver::components::ComponentContext& ctx);

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& request_context) const override;

 private:
  void EnableCORS(const userver::server::http::HttpRequest& request) const;
};

}  // namespace svh::video::components::http::post::uploader