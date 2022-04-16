.global SwitchMachineContext
SwitchMachineContext: # SwitchMachineContext(void** rsp_from, void* rsp_to)
# Callee-saved registers according to x86_64 ABI
# rbx, rsp, rbp, r12, r13, r14, r15

retq


.global SetupMachineContext
SetupMachineContext: # SetupMachineContext(void** stack_bottom, SpringboardFunction, void* SpringboardArg)

retq
