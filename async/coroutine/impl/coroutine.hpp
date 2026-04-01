#pragma once

#include <async/util/context/machine_context.hpp>
#include <async/util/memory/memory_mapping_view.hpp>

namespace async::coroutine::impl {

class Coroutine {
 public:
  template <class F>
  Coroutine(F routine, util::MemoryMappingView stack) {
    struct Runnable {
      F routine_;
      Coroutine& self_;

      [[noreturn]] void Run(void* payload) {
        routine_(payload);
        routine_.~F();
        self_.is_finished_ = true;
        self_.suspended_.ExitWith(nullptr);
      }
    };

    suspended_.Setup(stack.AsBytes(), Runnable{std::move(routine), *this});
  }

  // Context: Coroutine
  void* Suspend(void* payloat);

  // Context: Caller
  void* Resume(void* payload);

  [[nodiscard]] bool IsFinished() const;

 private:
  util::MemoryMappingView stack_;
  util::MachineContext suspended_;

  bool is_finished_ = false;
};

}  // namespace async::coroutine::impl
