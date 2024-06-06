#include "machine_context.hpp"

#include <cstdlib>

namespace async::util {

extern "C" void SetupMachineContext(void** rsp,
                                    void (*trampoline_function)(void*, void*,
                                                                void*, void*,
                                                                void*, void*,
                                                                void*),
                                    void* trampoline_argument);

extern "C" void SwitchMachineContext(void** from_rsp, void* to_rsp);

// First six arguments are passed through registers, seventh is passed through
// stack
[[noreturn]] void Trampoline(void*, void*, void*, void*, void*, void*,
                             void* arg7) {
  auto* springboard = static_cast<ITrampoline*>(arg7);
  springboard->Run();
  std::abort();
}

void MachineContext::Setup(MemoryMappingView stack, ITrampoline* trampoline) {
  rsp_ = static_cast<void*>(stack.End());
  SetupMachineContext(&rsp_, &Trampoline, static_cast<void*>(trampoline));
}

void MachineContext::SwitchTo(MachineContext& context) {
  SwitchMachineContext(&rsp_, context.rsp_);
}

}  // namespace async::util
