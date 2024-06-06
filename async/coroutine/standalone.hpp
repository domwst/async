#pragma once

#include <async/coroutine/impl/coroutine.hpp>
#include <async/util/memory/stack.hpp>

namespace async::coroutine {

class Coroutine {
 public:
  class SuspendHandle {
   public:
    void Suspend();

   private:
    friend Coroutine;

    explicit SuspendHandle(Coroutine&);

    Coroutine& self_;
  };

  using Routine = fu2::function<void(SuspendHandle)>;

  explicit Coroutine(Routine routine, size_t stack_pages = 8);

  void Resume();

  [[nodiscard]] bool IsFinished() const;

 private:
  void Suspend();

  util::Stack stack_;
  impl::Coroutine impl_;
};

}  // namespace async::coroutine
