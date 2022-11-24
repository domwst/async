#pragma once

#include "async/coroutine/impl/coroutine.hpp"
#include <async/util/memory/stack.hpp>
#include <async/util/defer.hpp>

#include <optional>
#include <cassert>

namespace async::coroutine {

struct GeneratorCanceled : std::exception {
};

template<class T>
class Generator {
 public:
  using Routine = impl::Coroutine::Routine;

  explicit Generator(Routine routine, size_t stack_pages = 8)
    : stack_(util::Stack::AllocateStack(stack_pages)),
      impl_(std::move(routine), stack_.View()) {
  }

  std::optional<T> Get() {
    ResumeImpl();
    return std::move(value_);
  }

  void Cancel() {
    assert(!is_canceled_);
    is_canceled_ = true;
    ResumeImpl();
  }

  //  Returns false if generator should stop
  static bool YieldNoThrow(T value) {
    Self().value_ = std::move(value);
    Self().impl_.Suspend();
    return !Self().is_canceled_;
  }

  static void Yield(T value) {
    if (!YieldNoThrow(std::move(value))) {
      throw GeneratorCanceled{};
    }
  }

  ~Generator() {
    if (!is_canceled_) {
      Cancel();
    }
  }

 private:
  void ResumeImpl() {
    auto prev = std::exchange(current_, this);
    util::Defer rollback([prev]() {
      current_ = prev;
    });

    try {
      impl_.Resume();
    } catch (GeneratorCanceled&) {
    }
  }

  static Generator& Self() {
    assert(current_ != nullptr);
    return *current_;
  }

  static Generator* current_;

  util::Stack stack_;
  impl::Coroutine impl_;
  bool is_canceled_{false};
  std::optional<T> value_;
};

template<class T>
Generator<T>* Generator<T>::current_ = nullptr;

}  // namespace async::coroutine
