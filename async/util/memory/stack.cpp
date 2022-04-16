#include "stack.hpp"

#include <utility>

namespace async::util {

Stack Stack::AllocateStack(size_t pages) noexcept {
  MemoryMapping memory = MemoryMapping::AllocatePages(pages + 1);
  memory.ProtectPages(0, 1);  // Guard page
  return Stack(std::move(memory));
}

Stack::Stack(MemoryMapping memory) noexcept: memory_(std::move(memory)) {
}

MemoryMappingView Stack::View() const noexcept {
  return memory_.View();
}

size_t Stack::Size() const noexcept {
  return memory_.Size() - MemoryMapping::PageSize;
}

}  // namespace async::util
