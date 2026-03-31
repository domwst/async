#pragma once

#include "async/coroutine/impl/coroutine.hpp"
#include <async/util/memory/stack.hpp>
#include <async/util/defer.hpp>

#include <optional>
#include <cassert>

namespace async::coroutine {

struct GeneratorCanceled : std::exception {};

template <class T>
class Generator {
 public:
  template <class F>
  explicit Generator(F routine, size_t stack_pages = 8)
      : stack_(util::Stack::AllocateStack(stack_pages)),
        impl_(
            [r = std::move(routine)] {
              try {
                r();
              } catch (GeneratorCanceled&) {
              }
            },
            stack_.View()) {
  }

  std::optional<T> Get() {
    void* payload = ResumeImpl();
    if (payload == nullptr) {
      return std::nullopt;
    }
    return std::move(*static_cast<T*>(payload));
  }

  void Cancel() {
    assert(!is_canceled_);
    is_canceled_ = true;
    ResumeImpl();
  }

  //  Returns false if generator should stop
  static bool YieldNoThrow(T value) {
    auto& self = Self();
    self.impl_.Suspend(&value);
    return !self.is_canceled_;
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
  void* ResumeImpl() {
    auto prev = std::exchange(current, this);
    util::Defer rollback([prev]() {
      current = prev;
    });

    try {
      return impl_.Resume(nullptr);
    } catch (GeneratorCanceled&) {
    }
    return nullptr;
  }

  static Generator& Self() {
    assert(current != nullptr);
    return *current;
  }

  static Generator* current;

  util::Stack stack_;
  impl::Coroutine impl_;
  bool is_canceled_{false};
};

template <class T>
Generator<T>* Generator<T>::current = nullptr;

}  // namespace async::coroutine
