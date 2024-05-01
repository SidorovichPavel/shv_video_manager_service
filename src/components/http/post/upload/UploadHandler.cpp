#include "UploadHandler.hpp"
#include "../../../upload/Uploader.hpp"
#include "fwd.hpp"

#include <charconv>

#include <userver/http/common_headers.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/component.hpp>
#include <userver/utils/assert.hpp>

namespace svh::video::components::http::post::upload {

UploadHandler::UploadHandler(const userver::components::ComponentConfig& cfg,
                             const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerBase(cfg, ctx),
      upload_controller_(
          ctx.FindComponent<::svh::video::components::upload::Uploader>()
              .GetUploadControllerPtr()) {}

std::string UploadHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
  EnableCORS(request);

  auto content_type = userver::http::ContentType(
      request.GetHeader(userver::http::headers::kContentType));

  if (content_type != userver::http::ContentType("multipart/form-data")) {
    LOG_DEBUG() << "Request content type: " << content_type << '\n';
    return MakeBadRequestValue(request,
                               "Expected 'multipart/form-data' content type");
  }

  LOG_DEBUG() << "Request payload size: "
              << request.GetArg("resumableCurrentChunkSize") << '\n';

  auto filename = request.GetArg("resumableFilename");
  auto identifier = request.GetArg("resumableIdentifier");

  auto resumableTotalChunks = request.GetArg("resumableTotalChunks");
  std::size_t total_chunks{};
  {
    auto [ptr, errc] = std::from_chars(
        resumableTotalChunks.data(),
        resumableTotalChunks.data() + resumableTotalChunks.size(),
        total_chunks);
    if (errc == std::errc::invalid_argument)
      return MakeBadRequestValue(request, "Invalid total chunks value");
  }

  auto resumableChunkNumber = request.GetArg("resumableChunkNumber");
  std::size_t chunk_number{};
  auto [ptr, errc] = std::from_chars(
      resumableChunkNumber.data(),
      resumableChunkNumber.data() + resumableChunkNumber.size(), chunk_number);
  if (errc == std::errc::invalid_argument)
    return MakeBadRequestValue(request, "Invalid chunk number value");

  const auto& file_data = request.GetFormDataArg("file");
  upload_controller_->push_block(filename, filename, total_chunks, chunk_number,
                                 file_data.value);

  return std::string("OK!\n");
}

std::string UploadHandler::MakeBadRequestValue(
    const userver::server::http::HttpRequest& request,
    std::string message) const {
  request.GetHttpResponse().SetStatus(
      userver::server::http::HttpStatus::kBadRequest);
  return message;
}

void UploadHandler::EnableCORS(
    const userver::server::http::HttpRequest& request) const {
  using namespace std::string_view_literals;

  request.GetHttpResponse().SetHeader("Access-Control-Allow-Origin"sv, "*");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Methods"sv,
                                      "OPTIONS, POST");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Headers"sv,
                                      "X-PINGOTHER, Content-Type");
}

void Append(userver::components::ComponentList& components_list) {
  components_list.Append<UploadHandler>();
}

}  // namespace svh::video::components::http::post::upload