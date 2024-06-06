#pragma once

#include <utility>

namespace async::util {

template <class TFunction>
class Defer {
 public:
  explicit Defer(TFunction function)
      : function_(std::move(function)) {
  }

  ~Defer() {
    function_();
  }

 private:
  TFunction function_;
};

}  // namespace async::util
