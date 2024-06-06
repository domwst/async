.global SwitchMachineContext
SwitchMachineContext:  # SwitchMachineContext(void** rsp_from, void* rsp_to)
# Callee-saved registers according to x86_64 ABI
# rbx, rsp, rbp, r12, r13, r14, r15

pushq %rbx
pushq %rbp
pushq %r12
pushq %r13
pushq %r14
pushq %r15

movq %rsp, (%rdi)
movq %rsi, %rsp

popq %r15
popq %r14
popq %r13
popq %r12
popq %rbp
popq %rbx

retq


.global SetupMachineContext
SetupMachineContext:  # SetupMachineContext(void** stack_bottom, TrampolineFunction, void* TrampolineArg)

movq %rsp, %rax
movq (%rdi), %rsp

pushq $0  # Stack alignment
pushq %rdx
pushq $0  # Fake return address
pushq %rsi

pushq $0  # %rbx
pushq $0  # %rbp
pushq $0  # %r12
pushq $0  # %r13
pushq $0  # %r14
pushq $0  # %r15

movq %rsp, (%rdi)
movq %rax, %rsp

retq
