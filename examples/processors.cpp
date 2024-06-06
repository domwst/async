#include <async/coroutine/processor.hpp>

#include <iostream>

using async::coroutine::Processor;

void Nested() {
  constexpr size_t kN = 10;
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

  for (size_t i = 0; i < kN; ++i) {
    sq_printer.Send(i);
  }
}

void MoveOnly() {
  class MoveOnlyType {
   public:
    explicit MoveOnlyType(size_t value)
        : value_(value) {
    }

    MoveOnlyType(const MoveOnlyType&) = delete;
    MoveOnlyType& operator=(const MoveOnlyType&) = delete;

    MoveOnlyType(MoveOnlyType&&) = default;
    MoveOnlyType& operator=(MoveOnlyType&&) = default;

    [[nodiscard]] size_t GetValue() const {
      return value_;
    }

   private:
    size_t value_;
  };

  Processor<MoveOnlyType> printer([]() {
    while (auto val = Processor<MoveOnlyType>::Receive()) {
      std::cout << val->GetValue() << std::endl;
    }
  });
  for (size_t i = 0; i < 10; ++i) {
    printer.Send(MoveOnlyType(i));
  }
};

int main() {
  MoveOnly();
}
