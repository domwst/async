#include "coroutine.hpp"


namespace async::coroutine::impl {

Coroutine::Coroutine(Coroutine::Routine routine, util::MemoryMappingView stack)
  : stack_(std::move(stack)), routine_(std::move(routine)) {

  callee_.Setup(stack_, this);
}

void Coroutine::Suspend() {
  callee_.SwitchTo(caller_);
}

void Coroutine::Resume() {
  caller_.SwitchTo(callee_);
  if (exception_) {
    std::rethrow_exception(exception_);
  }
}

void Coroutine::Run() {
  try {
    routine_();
  } catch (...) {
    exception_ = std::current_exception();
  }
  is_finished_ = true;
  Suspend();
  std::abort();
}

bool Coroutine::IsFinished() const {
  return is_finished_;
}

}  // namespace async::coroutine::impl
