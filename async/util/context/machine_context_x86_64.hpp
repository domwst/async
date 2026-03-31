#pragma once

#include "runnable.hpp"

#include <cassert>
#include <span>

namespace async::util::detail {

template <Runnable R>
void Trampoline(void*, void*, void*, void*, void*, void*, R runnable) {
  runnable.Run();
}

#pragma pack(push, 8)
template <Runnable R>
struct StackTop {
  void* rbp;
  void* trampoline;
  void* fake_return_address;
  R runnable;
};
#pragma pack(pop)

template <Runnable R>
void* SetupMachineContext(std::span<std::byte> stack, R runnable) {
  auto top = reinterpret_cast<size_t>(stack.data() + stack.size());
  constexpr size_t kStackAlignment = 16;
  assert(top % kStackAlignment == 0);

  top -= sizeof(R);
  top &= ~(alignof(StackTop<R>) - 1) & -kStackAlignment;
  top -= 24;
  // top -= offsetof(StackTop<R>, runnable);

  // I don't even begin to think what happens to R's move
  // constructor when the target is not aligned.
  return new (reinterpret_cast<void*>(top)) StackTop<R>{
      .rbp = nullptr,
      .trampoline = reinterpret_cast<void*>(Trampoline<R>),
      .fake_return_address = nullptr,
      .runnable = std::move(runnable),
  };
}

__attribute__((naked, used)) extern "C" inline void SwitchMachineContextImpl(
    void** target) {
  asm volatile(R"(
    push rbp
    mov rax, [rdi]
    mov [rdi], rsp
    mov rsp, rax
    pop rbp
    ret
  )");
}

static inline void* SwitchMachineContext(void** target, void* payload) {
  /*
   asm!(
     "call {switch_impl}",
         lateout("r12") _,
         lateout("r13") _,
         lateout("r14") _,
         lateout("r15") _,
         in("rdi") ctx,
         inlateout("rsi") payload,
         clobber_abi("sysv64"),
         switch_impl = sym switch_to_impl,
     );

    The following is a direct translation of above in a far inferior language.
  */
  asm volatile("call SwitchMachineContextImpl"
               : "+D"(target), "+S"(payload)::"rax", "rbx", "rcx", "rdx", "r8",
                 "r9", "r10", "r11", "r12", "r13", "r14", "r15", "xmm0", "xmm1",
                 "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9",
                 "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15", "cc",
                 "memory");
  return payload;
}

}  // namespace async::util::detail
