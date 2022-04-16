#pragma once

namespace async::util {

class ISpringBoard {
 public:
  [[noreturn]] virtual void Run() = 0;
};

}  // namespace async::util