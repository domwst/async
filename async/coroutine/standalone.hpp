#pragma once

#include "async/coroutine/impl/coroutine.hpp"
#include <async/util/memory/stack.hpp>

namespace async::coroutine {

class Coroutine {
 public:
  using Routine = impl::Coroutine::Routine;

  explicit Coroutine(Routine routine, size_t stack_pages = 8);

  void Resume();

  static void Suspend();

  [[nodiscard]] bool IsFinished() const;

 private:
  static Coroutine& Self();

  util::Stack stack_;
  impl::Coroutine impl_;
};

}
