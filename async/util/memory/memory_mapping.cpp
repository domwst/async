#include "memory_mapping.hpp"

#include <sys/mman.h>
#include <cassert>
#include <utility>

namespace async::util {

MemoryMapping::MemoryMapping(char* start, size_t size) noexcept
    : MemoryMappingView(start, size) {
}

MemoryMapping::MemoryMapping() noexcept
    : MemoryMappingView() {
}

MemoryMapping::MemoryMapping(MemoryMapping&& other) noexcept
    : MemoryMapping() {
  Swap(other);
}

MemoryMapping& MemoryMapping::operator=(MemoryMapping&& mapping) noexcept {
  MemoryMapping tmp(std::move(mapping));
  Swap(tmp);
  return *this;
}

MemoryMapping::~MemoryMapping() {
  if (Begin() != nullptr) {
    munmap(Begin(), Size());
  }
}

void MemoryMapping::ProtectPages(size_t first_page, size_t pages,
                                 int flags) noexcept {
  assert(mprotect(Data() + first_page * kPageSize, pages * kPageSize, flags) ==
         0);
}

MemoryMapping MemoryMapping::AllocatePages(size_t pages) noexcept {
  const size_t size = pages * kPageSize;
  void* mem = mmap(nullptr, size, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  assert(mem != MAP_FAILED);
  return {static_cast<char*>(mem), size};
}

MemoryMappingView MemoryMapping::View() const {
  return {Data(), Size()};
}

}  // namespace async::util
