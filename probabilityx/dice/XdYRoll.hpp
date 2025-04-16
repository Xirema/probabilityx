#pragma once
#include <boost/random.hpp>

#include "../Roll.hpp"

namespace probx::dice {
class XdYRoll {
  Integer numOfDice;
  Integer sizeOfDice;

 public:
  constexpr XdYRoll() : XdYRoll(1, 1) {}
  constexpr XdYRoll(Integer numOfDice, Integer sizeOfDice)
      : numOfDice(numOfDice), sizeOfDice(sizeOfDice) {}

  struct Iterator {
    using difference_type = Integer;
    using value_type = Outcome;
    Integer curr;
    constexpr Outcome operator*() const { return Outcome{curr}; }
    constexpr Iterator& operator++() {
      curr++;
      return *this;
    }
    constexpr Iterator& operator--() {
      curr--;
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
      curr += static_cast<Integer>(i);
      return *this;
    }
    constexpr Iterator& operator-=(size_t i) {
      curr -= static_cast<Integer>(i);
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
    constexpr Outcome operator[](Integer i) const { return Outcome{curr + i}; };
    constexpr Integer operator-(Iterator const& o) const {
      return curr - o.curr;
    }
    constexpr auto operator<=>(Iterator const& o) const = default;
  };

  constexpr Iterator begin() const { return Iterator{.curr = numOfDice}; }
  constexpr Iterator end() const {
    return Iterator{.curr = numOfDice * sizeOfDice + 1};
  }

  template <typename PRNG>
    requires std::uniform_random_bit_generator<std::remove_reference_t<PRNG>>
  constexpr Outcome operator()(PRNG&& engine) const {
    boost::random::uniform_int_distribution<BigInt> dist{0, trials() - 1};
    auto ret = dist(engine);
    for (auto outcome : *this) {
      auto odds = (*this)(outcome);
      ret -= odds;
      if (ret < 0) {
        return outcome;
      }
    }
    // This should never happen!
    return {};
  }

  constexpr static BigInt factorial(Integer v) {
    BigInt ret = 1;
    for (Integer i = 2; i <= v; i++) {
      ret *= i;
    }
    return ret;
  }

  constexpr static BigInt choose(Integer a, Integer b) {
    return factorial(a) / (factorial(b) * factorial(a - b));
  }

  constexpr BigInt operator()(Outcome o) const {
    if (o.extra != 0) {
      return 0;
    }
    if (o < *begin() || o >= *end()) {
      return 0;
    }
    BigInt ret = 0;
    for (Integer k = 0; k <= (o.result - numOfDice) / sizeOfDice; k++) {
      BigInt value =
          choose(numOfDice, k) * choose(o.result - sizeOfDice * k - 1,
                                        o.result - sizeOfDice * k - numOfDice);
      if (k % 2 == 0) {
        ret += value;
      } else {
        ret -= value;
      }
    }

    /*
    Integer index = o.result - (*begin()).result + 1;
    BigInt ret =
      factorial(index + numOfDice - 2) /
      ( factorial(index - 1) * factorial(numOfDice - 1))
      ;
    auto half = static_cast<Integer>(size() / 2);
    if(index > half) {
      ret -=
        numOfDice * factorial(index - half + numOfDice - 2) /
        ( factorial(index - half - 1) * factorial(numOfDice - 1))
        ;
    }
    */
    return ret;
  }

  constexpr size_t size() const {
    return static_cast<size_t>((*end()).result - (*begin()).result);
  }

  constexpr BigInt trials() const {
    BigInt ret = 1;
    for (int i = 1; i <= numOfDice; i++) {
      ret *= sizeOfDice;
    }
    return ret;
  }
};

static_assert(Rollable<XdYRoll>);
}  // namespace probx::dice
