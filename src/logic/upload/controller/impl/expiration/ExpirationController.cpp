#include "ExpirationController.hpp"

namespace svh::video::logic::upload::controller::impl::expiration {

ExpirationController::ExpirationController(
    std::chrono::system_clock::duration live_time) noexcept
    : live_time_(live_time),
      expiration_time_(std::chrono::system_clock::now() + live_time_) {}

bool ExpirationController::expired() const noexcept {
  return expiration_time_.load() < std::chrono::system_clock::now();
}

void ExpirationController::update() noexcept {
  expiration_time_.store(std::chrono::system_clock::now() + live_time_);
}

}  // namespace svh::video::logic::upload::controller::impl::expiration