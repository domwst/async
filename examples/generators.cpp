#include <async/coroutine/generator.hpp>
#include <async/util/defer.hpp>

#include <iostream>

using async::coroutine::Generator;
using async::util::Defer;

void Simple() {
  Generator<size_t> naturals([]() {
    size_t n = 0;
    Defer scope_guard([]() {
      std::cout << "Generator finished" << std::endl;
    });
    while (Generator<size_t>::YieldNoThrow(n++)) {}
  });

  for (size_t i = 0; i < 10; ++i) {
    naturals.Get();
  }
  naturals.Cancel();  // Should print "Generator finished" before "Test finished"
  std::cout << "Test finished" << std::endl;
}

void Nested() {
  Generator<size_t> ten_squares([]() {
    Generator<size_t> naturals([]() {
      size_t n = 0;
      while (Generator<size_t>::YieldNoThrow(n++)) {
      }
    });

    for (size_t i = 0; i < 10; ++i) {
      auto val = naturals.Get();
      if (!val) {
        return;
      }
      if (!Generator<size_t>::YieldNoThrow((*val) * (*val))) {
        break;
      }
    }
  });
  while (auto val = ten_squares.Get()) {
    std::cout << *val << std::endl;
  }
}

void MagicStop() {
  Generator<size_t> squares([]() {
    size_t n = 0;
    Defer scope_guard([]() {
      std::cout << "Generator finished" << std::endl;
    });
    while (true) {
      size_t val = n++;
      Generator<size_t>::Yield(val * val);
    }
  });
  for (size_t i = 0; i < 10; ++i) {
    auto val = squares.Get();
    if (!val) {
      break;
    }
    std::cout << *val << std::endl;
  }
  squares.Cancel();  // Should print "Generator finished" before "Test finished"
  std::cout << "Test finished" << std::endl;
}

void CancellationPropagation() {
  Generator<size_t> squares([]() {
    Defer scope_guard([]() {
      std::cout << "Squares generator scope finished" << std::endl;
    });

    Generator<size_t> naturals([]() {
      Defer scope_guard([]() {
        std::cout << "Naturals generator scope finished" << std::endl;
      });

      size_t n = 0;
      while (true) {
        Generator<size_t>::Yield(n++);
      }
    });

    while (true) {
      auto n = *naturals.Get();
      Generator<size_t>::Yield(n * n);
    }
  });

  for (size_t i = 0; i < 10; ++i) {
    std::cout << *squares.Get() << std::endl;
  }
}

void MoveOnly() {
  class MoveOnlyType {
   public:
    explicit MoveOnlyType(size_t value) : value_(value) {}

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

  Generator<MoveOnlyType> naturals([]() {
    for (size_t i = 0; i < 10; ++i) {
      Generator<MoveOnlyType>::Yield(MoveOnlyType(i));
    }
  });

  while (auto val = naturals.Get()) {
    std::cout << val->GetValue() << std::endl;
  }
}

int main() {
  CancellationPropagation();
}
