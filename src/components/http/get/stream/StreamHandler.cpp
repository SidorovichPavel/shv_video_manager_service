#include "StreamHandler.hpp"
#include "fwd.hpp"

#include <string>

#include <fmt/format.h>
#include <userver/components/component.hpp>
#include <userver/server/http/http_response_body_stream.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svh::video::components::http::get::stream {

StreamHandler::StreamHandler(const userver::components::ComponentConfig& cfg,
                             const userver::components::ComponentContext& ctx)
    : userver::server::handlers::HttpHandlerBase(cfg, ctx),
      fs_cache_client_(
          ctx.FindComponent<userver::components::FsCache>("stream-fs-cache")
              .GetClient()) {}

void StreamHandler::HandleStreamRequest(
    const userver::server::http::HttpRequest& request,
    [[maybe_unused]] userver::server::request::RequestContext& request_context,
    userver::server::http::ResponseBodyStream& response_body_stream) const {
  auto vid = request.GetPathArg("vid");
  auto object = request.GetPathArg("object");

  using namespace std::string_view_literals;
  response_body_stream.SetHeader("Accept-Ranges"sv, "bytes");
  response_body_stream.SetHeader("Access-Control-Allow-Origin"sv, "*");
  response_body_stream.SetHeader("Access-Control-Allow-Methods"sv, "GET");

  constexpr std::string_view kPathToObject = "/{}/{}";
  auto object_path = fmt::format(kPathToObject, vid, object);

  auto file_info = fs_cache_client_.TryGetFile(object_path);
  if (file_info == nullptr) {
    response_body_stream.SetStatusCode(
        userver::server::http::HttpStatus::kNotFound);
    return;
  }

  auto object_size = file_info->data.size();

  response_body_stream.SetHeader("Content-Length"sv,
                                 std::to_string(object_size));

  response_body_stream.SetStatusCode(userver::server::http::HttpStatus::kOk);
  response_body_stream.SetEndOfHeaders();

  response_body_stream.PushBodyChunk(std::string(file_info->data),
                                     userver::engine::Deadline{});
}

void Append(userver::components::ComponentList& componenet_list) {
  componenet_list.Append<StreamHandler>();
}
}  // namespace svh::video::components::http::get::stream