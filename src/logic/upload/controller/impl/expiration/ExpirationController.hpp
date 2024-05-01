#pragma once

#include <atomic>
#include <chrono>

namespace svh::video::logic::upload::controller::impl::expiration {

using namespace std::chrono_literals;
using duration = std::chrono::system_clock::duration;
using time_point = std::chrono::system_clock::time_point;

static constexpr duration k_default_expiration_time(10s);

class ExpirationController {
 public:
  ExpirationController(duration live_time = k_default_expiration_time) noexcept;

  ExpirationController(const ExpirationController&) = delete;
  ExpirationController(ExpirationController&&) = delete;

  ExpirationController& operator=(const ExpirationController&) = delete;
  ExpirationController& operator=(ExpirationController&&) = delete;

  ~ExpirationController() = default;

  bool expired() const noexcept;
  void update() noexcept;

 private:
  const duration live_time_;
  std::atomic<time_point> expiration_time_;
};

}  // namespace svh::video::logic::upload::controller::impl::expiration