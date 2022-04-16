#pragma once

#include <async/util/memory/memory_mapping_view.hpp>
#include <async/util/context/springboard.hpp>

namespace async::util {

class MachineContext {
 public:
  void Setup(MemoryMappingView stack, ISpringBoard* springboard);

  void SwitchTo(MachineContext& context);

 private:
  void* rsp{nullptr};
};

}  // namespace async::util
