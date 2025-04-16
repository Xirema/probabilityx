#pragma once
#include "../Roll.hpp"

namespace probx::dice {
class Modifier {
  Outcome mod;

 public:
  constexpr Modifier() : Modifier(0) {}
  constexpr Modifier(Outcome outcome) : mod(outcome) {}

  struct Iterator {
    using difference_type = Integer;
    using value_type = Outcome;
    Outcome curr;
    constexpr Outcome operator*() const { return curr; }
    constexpr Iterator& operator++() {
      curr.result++;
      return *this;
    }
    constexpr Iterator& operator--() {
      curr.result--;
      return *this;
    }
    constexpr Iterator operator++(int) {
      Iterator copy{*this};
      ++*this;
      return copy;
    }
    constexpr Iterator operator--(int) {
      Iterator copy{*this};
      --*this;
      return copy;
    }
    constexpr Iterator& operator+=(size_t i) {
      curr.result += static_cast<Integer>(i);
      return *this;
    }
    constexpr Iterator& operator-=(size_t i) {
      curr.result -= static_cast<Integer>(i);
      return *this;
    }
    constexpr Iterator operator+(size_t i) const {
      Iterator copy{*this};
      copy += static_cast<Integer>(i);
      return copy;
    }
    constexpr Iterator operator-(size_t i) const {
      Iterator copy{*this};
      copy -= static_cast<Integer>(i);
      return copy;
    }
    constexpr friend Iterator operator+(size_t i, Iterator o) { return o + i; }
    constexpr Outcome operator[](Integer i) const {
      return Outcome{curr.result + i, curr.extra};
    };
    constexpr Integer operator-(Iterator const& o) const {
      return curr.result - o.curr.result;
    }
    constexpr auto operator<=>(Iterator const& o) const = default;
  };

  constexpr Iterator begin() const { return Iterator{.curr = mod}; }
  constexpr Iterator end() const {
    return Iterator{.curr = Outcome{mod.result + 1, mod.extra}};
  }

  template <typename PRNG>
    requires std::uniform_random_bit_generator<std::remove_reference_t<PRNG>>
  constexpr Outcome operator()(PRNG&& engine) const {
    return mod;
  }

  constexpr BigInt operator()(Outcome o) const { return (mod == o) ? 1 : 0; }

  constexpr size_t size() const { return 1; }
  constexpr BigInt trials() const { return 1; }
};

static_assert(Rollable<Modifier>);
}  // namespace probx::dice
