#include "coroutine_impl.hpp"

// You may need to twist your brain into a knot to write this

namespace async::coroutine::detail {


CoroutineImpl::CoroutineImpl(CoroutineImpl::Routine /*routine*/, util::MemoryMappingView /*stack*/) {
}

void CoroutineImpl::Suspend() {
}

void CoroutineImpl::Resume() {
}

void CoroutineImpl::Run() {
}

bool CoroutineImpl::IsFinished() const {
  return false;
}

}  // namespace async::coroutine::detail