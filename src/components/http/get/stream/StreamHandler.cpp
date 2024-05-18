#include "StreamHandler.hpp"
#include "fwd.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include <userver/server/http/http_response_body_stream.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svh::video::components::http::get::stream {

enum class ContentType { audio_mp4, video_mp4 };

constexpr userver::utils::TrivialBiMap kContentTypeDescriptor =
    [](auto selector) {
      return selector()
          .Case(ContentType::audio_mp4, "audio/mp4")
          .Case(ContentType::video_mp4, "video/mp4");
    };

StreamHandler::StreamHandler(const userver::components::ComponentConfig& cfg,
                             const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerBase(cfg, ctx) {}

void StreamHandler::HandleStreamRequest(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& request_context,
    userver::server::http::ResponseBodyStream& response_body_stream) const {
  auto vid = request.GetPathArg("vid");
  auto object = request.GetPathArg("object");

  using namespace std::string_view_literals;
  response_body_stream.SetHeader("Accept-Ranges"sv, "bytes");
  response_body_stream.SetHeader("Access-Control-Allow-Origin"sv, "*");
  response_body_stream.SetHeader("Access-Control-Allow-Methods"sv, "GET");

  auto object_path = std::filesystem::path("/tmp/streams") / vid / object;
  auto object_size = std::filesystem::file_size(object_path);

  response_body_stream.SetHeader("Content-Length"sv,
                                 std::to_string(object_size));
  // response_body_stream.SetHeader("Content-Type"sv, );

  response_body_stream.SetStatusCode(userver::server::http::HttpStatus::kOk);
  response_body_stream.SetEndOfHeaders();

  std::ifstream fin(object_path, std::ios::binary);
  if (!fin.is_open()) {
    response_body_stream.SetStatusCode(
        userver::server::http::HttpStatus::kBadRequest);
    return;
  }

  std::string data;
  data.resize(object_size);
  constexpr std::streamsize buffer_size = 1024;
  fin.read(data.data(), object_size);
  response_body_stream.PushBodyChunk(std::move(data),
                                     userver::engine::Deadline{});
}

std::size_t StreamHandler::GetStreamId(std::string_view stream) const {}

void Append(userver::components::ComponentList& componenet_list) {
  componenet_list.Append<StreamHandler>();
}
}  // namespace svh::video::components::http::get::stream