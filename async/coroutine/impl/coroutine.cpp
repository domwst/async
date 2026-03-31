#include "coroutine.hpp"

namespace async::coroutine::impl {

void* Coroutine::Suspend(void* payload) {
  return suspended_.ActivateWith(payload);
}

void* Coroutine::Resume(void* payload) {
  return suspended_.ActivateWith(payload);
}

bool Coroutine::IsFinished() const {
  return is_finished_;
}

}  // namespace async::coroutine::impl
