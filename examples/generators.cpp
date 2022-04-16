#include <async/coroutine/generator.hpp>

#include <iostream>

using async::coroutine::Generator;

void Nested() {
  Generator<size_t> ten_squares([]() {
    Generator<size_t> naturals([]() {
      size_t n = 0;
      while (Generator<size_t>::Yield(n++)) {
      }
    });

    for (size_t i = 0; i < 10; ++i) {
      auto val = naturals.Get();
      if (!val) {
        return;
      }
      if (!Generator<size_t>::Yield((*val) * (*val))) {
        break;
      }
    }
  });
  while (auto val = ten_squares.Get()) {
    std::cout << *val << std::endl;
  }
}

void MagicStop() {
  Generator<size_t> naturals([]() {
    size_t n = 0;
    while (true) {
      size_t val = n++;
      Generator<size_t>::Yield2(val * val);
    }
  });
  for (size_t i = 0; i < 10; ++i) {
    std::cout << *naturals.Get() << std::endl;
  }
  naturals.Cancel();
}

int main() {
  MagicStop();
}