#include "machine_context.hpp"

#include <cstdlib>

namespace async::util {

namespace detail {

extern "C" void SetupMachineContext(
  void** rsp,
  void(*SpringboardFunction)(void*, void*, void*, void*, void*, void*, void*),
  void* SpringboardArgument);

extern "C" void SwitchMachineContext(void** from_rsp, void* to_rsp);

// First six arguments are passed through registers, seventh is passed through stack
[[noreturn]] void SpringBoard(void*, void*, void*, void*, void*, void*, void* arg7) {
  auto* springboard = static_cast<ISpringBoard*>(arg7);
  springboard->Run();
  std::abort();
}

}  // namespace detail

void MachineContext::Setup(MemoryMappingView stack, ISpringBoard* springboard) {
  rsp = static_cast<void*>(stack.End());
  detail::SetupMachineContext(&rsp, &detail::SpringBoard, static_cast<void*>(springboard));
}

void MachineContext::SwitchTo(MachineContext& context) {
  detail::SwitchMachineContext(&rsp, context.rsp);
}

}  // namespace async::util
