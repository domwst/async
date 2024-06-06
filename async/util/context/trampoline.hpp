#pragma once

namespace async::util {

class ITrampoline {
 public:
  [[noreturn]] virtual void Run() = 0;
};

}  // namespace async::util
