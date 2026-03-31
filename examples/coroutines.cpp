#include <async/coroutine/standalone.hpp>

#include <iostream>

using async::coroutine::Coroutine;

void Simple() {
  Coroutine coroutine1([](Coroutine::SuspendHandle self) {
    std::cout << 2 << std::endl;
    self.Suspend(nullptr);
    std::cout << 5 << std::endl;
  });

  Coroutine coroutine2([](Coroutine::SuspendHandle self) {
    std::cout << 3 << std::endl;
    self.Suspend(nullptr);
    std::cout << 6 << std::endl;
  });

  std::cout << 1 << std::endl;
  coroutine1.Resume(nullptr);
  coroutine2.Resume(nullptr);

  std::cout << 4 << std::endl;
  coroutine1.Resume(nullptr);
  coroutine2.Resume(nullptr);
}

void Nested() {
  Coroutine a([](Coroutine::SuspendHandle self) {
    Coroutine b([](Coroutine::SuspendHandle self) {
      Coroutine c([](Coroutine::SuspendHandle self) {
        std::cout << 4 << std::endl;
        self.Suspend(nullptr);
        std::cout << 8 << std::endl;
      });

      std::cout << 3 << std::endl;
      c.Resume(nullptr);
      self.Suspend(nullptr);

      std::cout << 7 << std::endl;
      c.Resume(nullptr);
    });
    std::cout << 2 << std::endl;
    b.Resume(nullptr);
    self.Suspend(nullptr);
    std::cout << 6 << std::endl;
    b.Resume(nullptr);
  });

  std::cout << 1 << std::endl;
  a.Resume(nullptr);

  std::cout << 5 << std::endl;
  a.Resume(nullptr);
}

int main() {
  Simple();
  Nested();
}
