#pragma once

#include <cstddef>

namespace async::util {

class MemoryMappingView {
 public:
  MemoryMappingView() noexcept;

  MemoryMappingView(char* start, size_t size) noexcept;

  MemoryMappingView(const MemoryMappingView&) noexcept = default;

  MemoryMappingView& operator=(const MemoryMappingView&) noexcept = default;

  MemoryMappingView(MemoryMappingView&&) noexcept;

  MemoryMappingView& operator=(MemoryMappingView&&) noexcept;

  void Swap(MemoryMappingView& other) noexcept;

  [[nodiscard]] char* Begin() const noexcept;

  [[nodiscard]] char* End() const noexcept;

  [[nodiscard]] char* Data() const noexcept;

  [[nodiscard]] size_t Size() const noexcept;

  static const size_t kPageSize;

 private:
  char* start_;
  size_t size_;
};

}  // namespace async::util
