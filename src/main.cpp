#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "components/convert/fwd.hpp"
#include "components/http/options/fwd.hpp"
#include "components/http/post/upload/fwd.hpp"
#include "components/upload/fwd.hpp"
#include "hello.hpp"

#include <userver/cache/base_postgres_cache_fwd.hpp>

int main(int argc, char *argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::server::handlers::TestsControl>();

  svh_video_service::AppendHello(component_list);
  svh::video::components::http::post::upload::Append(component_list);
  svh::video::components::http::options::Append(component_list);
  svh::video::components::upload::Append(component_list);
  svh::video::components::convert::Append(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}