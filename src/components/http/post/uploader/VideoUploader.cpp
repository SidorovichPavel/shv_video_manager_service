#include "VideoUploader.hpp"
#include "fwd.hpp"

#include <userver/http/common_headers.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/component.hpp>
#include <userver/utils/assert.hpp>

namespace svh::video::components::http::post::uploader {

VideoUploader::VideoUploader(const userver::components::ComponentConfig& cfg,
                             const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerBase(cfg, ctx) {}

std::string VideoUploader::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
  auto content_type = userver::http::ContentType(
      request.GetHeader(userver::http::headers::kContentType));

  LOG_DEBUG() << "Request content type: " << content_type << '\n';
  LOG_DEBUG() << "Request payload size: "
              << request.GetArg("resumableCurrentChunkSize") << '\n';
  // request.GetFormDataArg("");
  return std::string("OK!\n");
}

void VideoUploader::EnableCORS(
    const userver::server::http::HttpRequest& request) const {
  using namespace std::string_view_literals;

  request.GetHttpResponse().SetHeader("Access-Control-Allow-Origin"sv, "*");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Methods"sv,
                                      "OPTIONS, POST");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Headers"sv,
                                      "X-PINGOTHER, Content-Type");
}

void Append(userver::components::ComponentList& components_list) {
  components_list.Append<VideoUploader>();
}

}  // namespace svh::video::components::http::post::uploader