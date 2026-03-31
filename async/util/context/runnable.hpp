#pragma once

#include <concepts>

namespace async::util {

template <class T>
concept Runnable = requires(T t) {
  { t.Run() } -> std::same_as</* noreturn */ void>;
};

}  // namespace async::util
