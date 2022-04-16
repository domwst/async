#pragma once

#include <async/coroutine/coroutine_impl.hpp>
#include <async/util/memory/stack.hpp>

namespace async::coroutine {

class Coroutine {
 public:
  using Routine = detail::CoroutineImpl::Routine;

  explicit Coroutine(Routine routine, size_t stack_pages = 8);

  static Coroutine& Self();

  void Resume();

  static void Suspend();

 private:
  util::Stack stack_;
  detail::CoroutineImpl impl_;
};

}