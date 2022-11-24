#pragma once

#include <async/coroutine/impl/coroutine.hpp>
#include <async/util/memory/stack.hpp>
#include <async/util/defer.hpp>

#include <optional>
#include <cassert>

namespace async::coroutine {

template<class T>
class Processor {
 public:
  using Routine = impl::Coroutine::Routine;

  explicit Processor(Routine routine, size_t stack_pages = 8){
  }

  // Context: Coroutine
  static std::optional<T> Receive() {
    return {};
  }

  // Context: Caller
  void Send(T /*value*/) {
  }

  [[nodiscard]] bool IsClosed() const {
    return false;
  }

  void Close() {
  }

 private:
  static std::optional<T> value_;
};

template<class T>
std::optional<T> Processor<T>::value_{};

}  // namespace async::coroutine
