#include <async/coroutine/processor.hpp>

#include <iostream>

using async::coroutine::Processor;

int main() {
  constexpr size_t n = 10;
  Processor<size_t> sq_printer([]() {
    Processor<size_t> printer([]() {
      while (auto val = Processor<size_t>::Receive()) {
        std::cout << *val << std::endl;
      }
    });

    while (auto val = Processor<size_t>::Receive()) {
      printer.Send((*val) * (*val));
    }
  });

  for (size_t i = 0; i < n; ++i) {
    sq_printer.Send(i);
  }
}