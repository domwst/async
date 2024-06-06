#pragma once

#include "trampoline.hpp"

#include <async/util/memory/memory_mapping_view.hpp>

namespace async::util {

class MachineContext {
 public:
  void Setup(MemoryMappingView stack, ITrampoline* trampoline);

  void SwitchTo(MachineContext& context);

 private:
  void* rsp_{nullptr};
};

}  // namespace async::util
