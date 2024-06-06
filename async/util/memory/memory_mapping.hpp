#pragma once

#include <async/util/memory/memory_mapping_view.hpp>

#include <cstddef>
#include <sys/mman.h>

namespace async::util {

class MemoryMapping : public MemoryMappingView {
 public:
  MemoryMapping() noexcept;

  MemoryMapping(MemoryMapping&&) noexcept;
  MemoryMapping& operator=(MemoryMapping&&) noexcept;

  MemoryMapping(const MemoryMapping&) = delete;
  MemoryMapping& operator=(const MemoryMapping&) = delete;

  ~MemoryMapping();

  void ProtectPages(size_t first_page, size_t pages,
                    int flags = PROT_NONE) noexcept;

  [[nodiscard]] static MemoryMapping AllocatePages(size_t pages) noexcept;

  [[nodiscard]] MemoryMappingView View() const;

 private:
  MemoryMapping(char* start, size_t size) noexcept;
};

}  // namespace async::util
