#pragma once

#include "impl/coroutine.hpp"

#include <async/util/memory/stack.hpp>

namespace async::coroutine {

class Coroutine {
 public:
  class SuspendHandle {
   public:
    void* Suspend(void* payload);

   private:
    friend Coroutine;

    explicit SuspendHandle(Coroutine&);

    Coroutine& self_;
  };

  template <class F>
  explicit Coroutine(F routine, size_t stack_pages = 8)
      : stack_(util::Stack::AllocateStack(stack_pages)),
        impl_(
            [this, r = std::move(routine)] {
              r(SuspendHandle{*this});
            },
            stack_.View()) {
  }

  void* Resume(void* payload);

  [[nodiscard]] bool IsFinished() const;

 private:
  void* Suspend(void* payload);

  util::Stack stack_;
  impl::Coroutine impl_;
};

}  // namespace async::coroutine
