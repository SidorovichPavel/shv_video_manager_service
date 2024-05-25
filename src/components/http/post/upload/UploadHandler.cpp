#include "UploadHandler.hpp"
#include <UploadComponent.hpp>
#include "fwd.hpp"

#include <charconv>

#include <userver/formats/json.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/component.hpp>
#include <userver/utils/assert.hpp>

namespace svh::video::components::http::post::upload {

UploadHandler::UploadHandler(const userver::components::ComponentConfig& cfg,
                             const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerBase(cfg, ctx),
      upload_controller_(
          ctx.FindComponent<::svh::video::components::upload::UploadComponent>()
              .GetUploadControllerPtr()) {}

std::string UploadHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    [[maybe_unused]] userver::server::request::RequestContext& request_context)
    const {
  EnableCORS(request);

  auto content_type = userver::http::ContentType(
      request.GetHeader(userver::http::headers::kContentType));

  if (content_type != userver::http::ContentType("multipart/form-data")) {
    LOG_DEBUG() << "Request content type: " << content_type << '\n';
    request.GetHttpResponse().SetStatus(
        userver::server::http::HttpStatus::kBadRequest);
    return std::string{};
  }

  std::size_t total_chunks = GetTotalChunks(request);
  std::size_t chunk_number = GetChunkNumber(request);
  LOG_DEBUG() << total_chunks << chunk_number;
  if (total_chunks == 0u || chunk_number == 0u) {
    request.GetHttpResponse().SetStatus(
        userver::server::http::HttpStatus::kBadRequest);
    return std::string{};
  }

  std::string_view json = request.GetFormDataArg("json2").value;
  LOG_DEBUG() << json;
  auto upload_token = userver::formats::json::FromString(json);
  if (!upload_token.HasMember("vid")) {
    request.GetHttpResponse().SetStatus(
        userver::server::http::HttpStatus::kBadRequest);
    return std::string{};
  }

  const auto& file_data = request.GetFormDataArg("file");
  upload_controller_->push_block(std::to_string(upload_token["vid"].As<std::int64_t>()),
                                 total_chunks, chunk_number, file_data.value);

  return std::string{};
}

void UploadHandler::EnableCORS(
    const userver::server::http::HttpRequest& request) const {
  using namespace std::string_view_literals;

  request.GetHttpResponse().SetHeader("Access-Control-Allow-Origin"sv, "*");
  request.GetHttpResponse().SetHeader("Access-Control-Allow-Methods"sv,
                                      "OPTIONS, POST, GET");
}

std::size_t UploadHandler::GetTotalChunks(
    const userver::server::http::HttpRequest& request) const {
  std::string_view arg = request.GetArg("resumableTotalChunks");
  return StrToSizeT(arg);
}

std::size_t UploadHandler::GetChunkNumber(
    const userver::server::http::HttpRequest& request) const {
  std::string_view arg = request.GetArg("resumableChunkNumber");
  return StrToSizeT(arg);
}

std::size_t UploadHandler::StrToSizeT(std::string_view arg) const {
  std::size_t value{};
  std::ignore = std::from_chars(arg.begin(), arg.end(), value);
  return value;
}

void Append(userver::components::ComponentList& components_list) {
  components_list.Append<UploadHandler>();
}

}  // namespace svh::video::components::http::post::upload