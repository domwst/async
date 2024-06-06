#include "machine_context.hpp"

#include <cstdlib>

namespace async::util {

extern "C" void SetupMachineContext(
  void** rsp,
  void(*TrampolineFunction)(void*, void*, void*, void*, void*, void*, void*),
  void* TrampolineArgument);

extern "C" void SwitchMachineContext(void** from_rsp, void* to_rsp);

// First six arguments are passed through registers, seventh is passed through stack
[[noreturn]] void Trampoline(void*, void*, void*, void*, void*, void*, void* arg7) {
  auto* springboard = static_cast<ITrampoline*>(arg7);
  springboard->Run();
  std::abort();
}

void MachineContext::Setup(MemoryMappingView stack, ITrampoline* trampoline) {
  rsp = static_cast<void*>(stack.End());
  SetupMachineContext(&rsp, &Trampoline, static_cast<void*>(trampoline));
}

void MachineContext::SwitchTo(MachineContext& context) {
  SwitchMachineContext(&rsp, context.rsp);
}

}  // namespace async::util
