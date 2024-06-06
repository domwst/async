#include "standalone.hpp"

#include <async/util/defer.hpp>
#include <cassert>

namespace async::coroutine {

Coroutine::SuspendHandle::SuspendHandle(Coroutine& self)
    : self_(self) {
}

void Coroutine::SuspendHandle::Suspend() {
  self_.Suspend();
}

Coroutine::Coroutine(Coroutine::Routine routine, size_t stack_pages)
    : stack_(util::Stack::AllocateStack(stack_pages)),
      impl_(
          [this, routine = std::move(routine)]() mutable {
            routine(SuspendHandle(*this));
          },
          stack_.View()) {
}

void Coroutine::Resume() {
  impl_.Resume();
}

void Coroutine::Suspend() {
  impl_.Suspend();
}

bool Coroutine::IsFinished() const {
  return impl_.IsFinished();
}

}  // namespace async::coroutine
