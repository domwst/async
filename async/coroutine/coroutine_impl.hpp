#pragma once

#include <functional>
#include <exception>

#include <async/util/context/machine_context.hpp>
#include <async/util/memory/memory_mapping_view.hpp>

namespace async::coroutine::detail {

class CoroutineImpl : public util::ISpringBoard {
 public:
  using Routine = std::function<void()>;

  CoroutineImpl(Routine routine, util::MemoryMappingView stack);

  // Context: Coroutine
  void Suspend();

  // Context: Caller
  void Resume();

  [[nodiscard]] bool IsFinished() const;

 private:
  [[noreturn]] void Run() override;
};

}  // namespace async::coroutine::detail