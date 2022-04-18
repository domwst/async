#include <async/coroutine/generator.hpp>
#include <async/util/defer.hpp>

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
    async::util::Defer scope_guard([]() {
      std::cout << "Finished" << std::endl;
    });
    while (true) {
      size_t val = n++;
      Generator<size_t>::Yield2(val * val);
    }
  });
  for (size_t i = 0; i < 10; ++i) {
    auto val = naturals.Get();
    if (!val) {
      break;
    }
    std::cout << *val << std::endl;
  }
  naturals.Cancel();
  // Should print "Finished"
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
  MagicStop();
}