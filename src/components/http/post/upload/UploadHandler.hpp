#include <string_view>

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <controller/UploadController.hpp>

namespace svh::video::components::http::post::upload {

class UploadHandler final : public userver::server::handlers::HttpHandlerBase {
 public:
  constexpr static std::string_view kName = "video-uploader";

  UploadHandler(const userver::components::ComponentConfig& cfg,
                const userver::components::ComponentContext& ctx);

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& request_context) const override;

 private:
  void EnableCORS(const userver::server::http::HttpRequest& request) const;

  std::string MakeBadRequestValue(
      const userver::server::http::HttpRequest& request,
      std::string message) const;

  std::shared_ptr<logic::upload::controller::UploadController>
      upload_controller_;
};

}  // namespace svh::video::components::http::post::upload