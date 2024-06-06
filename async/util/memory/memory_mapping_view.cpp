#include "memory_mapping_view.hpp"

#include <utility>

#include <unistd.h>

namespace async::util {

MemoryMappingView::MemoryMappingView() noexcept
    : MemoryMappingView(nullptr, 0) {
}

MemoryMappingView::MemoryMappingView(char* start, size_t size) noexcept
    : start_(start),
      size_(size) {
}

MemoryMappingView::MemoryMappingView(MemoryMappingView&& other) noexcept
    : MemoryMappingView() {
  Swap(other);
}

MemoryMappingView& MemoryMappingView::operator=(
    MemoryMappingView&& other) noexcept {
  MemoryMappingView tmp(std::move(other));
  Swap(tmp);
  return *this;
}

void MemoryMappingView::Swap(MemoryMappingView& other) noexcept {
  std::swap(start_, other.start_);
  std::swap(size_, other.size_);
}

const size_t MemoryMappingView::kPageSize = sysconf(_SC_PAGESIZE);

char* MemoryMappingView::Begin() const noexcept {
  return start_;
}

char* MemoryMappingView::End() const noexcept {
  return start_ + size_;
}

char* MemoryMappingView::Data() const noexcept {
  return start_;
}

size_t MemoryMappingView::Size() const noexcept {
  return size_;
}

}  // namespace async::util
