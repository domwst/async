#include "standalone.hpp"

#include <async/util/defer.hpp>
#include <cassert>

namespace async::coroutine {

static thread_local Coroutine* current = nullptr;

Coroutine::Coroutine(Coroutine::Routine routine, size_t stack_pages)
  : stack_(util::Stack::AllocateStack(stack_pages)),
    impl_(std::move(routine), stack_.View()) {
}

Coroutine& Coroutine::Self() {
  assert(current != nullptr);
  return *current;
}

void Coroutine::Resume() {
  Coroutine* prev = std::exchange(current, this);

  util::Defer rollback([prev]() {
    current = prev;
  });

  impl_.Resume();
}

void Coroutine::Suspend() {
  Self().impl_.Suspend();
}

bool Coroutine::IsFinished() const {
  return impl_.IsFinished();
}

}
