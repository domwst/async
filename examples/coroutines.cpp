#include <async/coroutine/standalone.hpp>

#include <iostream>

using async::coroutine::Coroutine;

void Simple() {
  Coroutine coroutine1([](Coroutine::SuspendHandle self) {
    std::cout << 2 << std::endl;
    self.Suspend();
    std::cout << 5 << std::endl;
  });

  Coroutine coroutine2([](Coroutine::SuspendHandle self) {
    std::cout << 3 << std::endl;
    self.Suspend();
    std::cout << 6 << std::endl;
  });

  std::cout << 1 << std::endl;
  coroutine1.Resume();
  coroutine2.Resume();

  std::cout << 4 << std::endl;
  coroutine1.Resume();
  coroutine2.Resume();
}

void Nested() {
  Coroutine a([](Coroutine::SuspendHandle self) {
    Coroutine b([](Coroutine::SuspendHandle self) {

      Coroutine c([](Coroutine::SuspendHandle self) {
        std::cout << 4 << std::endl;
        self.Suspend();
        std::cout << 8 << std::endl;
      });

      std::cout << 3 << std::endl;
      c.Resume();
      self.Suspend();

      std::cout << 7 << std::endl;
      c.Resume();

    });
    std::cout << 2 << std::endl;
    b.Resume();
    self.Suspend();
    std::cout << 6 << std::endl;
    b.Resume();
  });

  std::cout << 1 << std::endl;
  a.Resume();

  std::cout << 5 << std::endl;
  a.Resume();

}

int main() {
  Simple();
  Nested();
}
