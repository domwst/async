#pragma once

#include <async/util/memory/memory_mapping.hpp>
#include <async/util/memory/memory_mapping_view.hpp>

namespace async::util {

class Stack {
 public:
  explicit Stack(MemoryMapping memory) noexcept;

  Stack(const Stack&) = delete;
  Stack& operator=(const Stack&) = delete;

  Stack(Stack&&) noexcept = default;
  Stack& operator=(Stack&&) noexcept = default;

  [[nodiscard]] size_t Size() const noexcept;

  [[nodiscard]] MemoryMappingView View() const noexcept;

  static Stack AllocateStack(size_t pages) noexcept;

 private:
  async::util::MemoryMapping memory_;
};

}  // namespace async::util
