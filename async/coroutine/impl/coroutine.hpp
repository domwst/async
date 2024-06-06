#pragma once

#include <functional>
#include <exception>

#include <async/util/context/machine_context.hpp>
#include <async/util/memory/memory_mapping_view.hpp>
#include <async/util/unique_function.hpp>

namespace async::coroutine::impl {

class Coroutine : public util::ITrampoline {
 public:
  using Routine = UniqueFunction<void()>;

  Coroutine(Routine routine, util::MemoryMappingView stack);

  // Context: Coroutine
  void Suspend();

  // Context: Caller
  void Resume();

  [[nodiscard]] bool IsFinished() const;

 private:
  [[noreturn]] void Run() override;

  util::MemoryMappingView stack_;

  // Because of the implementation, one MachineContext can be used
  // Two MachineContext's are here just for better readability
  util::MachineContext caller_;
  util::MachineContext callee_;

  Routine routine_;
  bool is_finished_ = false;
  std::exception_ptr exception_;
};

}  // namespace async::coroutine::impl
