#pragma once

#include "machine_context_x86_64.hpp"

#include "runnable.hpp"

#include <cstdlib>
#include <span>

namespace async::util {

class MachineContext {
 public:
  template <Runnable R>
  void Setup(std::span<std::byte> stack, R runnable) {
    rsp_ = detail::SetupMachineContext<R>(stack, std::move(runnable));
  }

  void* ActivateWith(void* payload) {
    return detail::SwitchMachineContext(&rsp_, payload);
  }

  [[noreturn]] void ExitWith(void* payload) {
    ActivateWith(payload);
    std::abort();
  }

 private:
  void* rsp_{nullptr};
};

}  // namespace async::util
