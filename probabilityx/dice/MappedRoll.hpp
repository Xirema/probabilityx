#pragma once
#include <boost/random.hpp>

#include "../Roll.hpp"

namespace probx::dice {
class MappedRoll {
 public:
  using Map = std::map<Outcome, BigInt>;

 private:
  Map map;
  BigInt memoizedTrials;

 public:
  constexpr MappedRoll(Map map = {}) : map(std::move(map)), memoizedTrials(0) {
    for (auto const& [outcome, odds] : this->map) {
      memoizedTrials += odds;
    }
  }

  constexpr MappedRoll(Rollable auto&& roll) {
    for (auto outcome : roll) {
      BigInt trials = roll(outcome);
      map[outcome] += trials;
      memoizedTrials += trials;
    }
  }

  struct Iterator {
    Map::const_iterator curr;
    using difference_type = Map::const_iterator::difference_type;
    using value_type = Outcome;
    constexpr Outcome operator*() const { return curr->first; }
    constexpr Iterator& operator--() {
      --curr;
      return *this;
    }
    constexpr Iterator& operator++() {
      ++curr;
      return *this;
    }
    constexpr Iterator operator--(int) {
      Iterator copy{*this};
      --*this;
      return copy;
    }
    constexpr Iterator operator++(int) {
      Iterator copy{*this};
      ++*this;
      return copy;
    }
    constexpr auto operator<=>(Iterator const& o) const = default;
  };

  constexpr Iterator begin() const { return Iterator{.curr = map.begin()}; }
  constexpr Iterator end() const { return Iterator{.curr = map.end()}; }

  template <typename PRNG>
    requires std::uniform_random_bit_generator<std::remove_reference_t<PRNG>>
  constexpr Outcome operator()(PRNG&& engine) const {
    boost::random::uniform_int_distribution<BigInt> dist{0, trials() - 1};
    BigInt ret = dist(engine);
    for (auto const& [outcome, odds] : map) {
      if (ret <= odds) {
        return outcome;
      }
      ret -= odds;
    }
    // Should never happen
    return {};
  }

  constexpr BigInt operator()(Outcome o) const {
    if (auto it = map.find(o); it != map.end()) {
      return it->second;
    }
    return 0;
  }

  constexpr size_t size() const { return map.size(); }

  constexpr BigInt trials() const { return memoizedTrials; }
};
static_assert(std::bidirectional_iterator<MappedRoll::Iterator>);
static_assert(Rollable<MappedRoll>);
}  // namespace probx::dice
