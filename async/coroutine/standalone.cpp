#include "standalone.hpp"

#include <async/util/defer.hpp>
#include <cassert>

namespace async::coroutine {

Coroutine::SuspendHandle::SuspendHandle(Coroutine& self)
    : self_(self) {
}

void* Coroutine::SuspendHandle::Suspend(void* payload) {
  return self_.Suspend(payload);
}

void* Coroutine::Resume(void* payload) {
  return impl_.Resume(payload);
}

void* Coroutine::Suspend(void* payload) {
  return impl_.Suspend(payload);
}

bool Coroutine::IsFinished() const {
  return impl_.IsFinished();
}

}  // namespace async::coroutine
