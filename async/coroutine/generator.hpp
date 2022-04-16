#pragma once

#include <async/coroutine/coroutine_impl.hpp>
#include <async/util/memory/stack.hpp>
#include <async/util/defer.hpp>

#include <optional>
#include <cassert>

namespace async::coroutine {

class StopGenerating : public std::exception {
};

template<class T>
class Generator {
 public:
  using Routine = detail::CoroutineImpl::Routine;

  explicit Generator(Routine /*routine*/, size_t /*stack_pages*/ = 8){
  }

  std::optional<T> Get() {
    return {};
  }

  static bool Yield(T value) {
    return true;
  }

  static void Yield2(T value) {
  }

  void Cancel() {
  }
};

}  // namespace async::coroutine