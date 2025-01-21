#pragma once
#include "../Roll.hpp"

namespace probx::dice {
  class CustomDie {
    std::vector<Outcome> outcomes;
  public:
    constexpr CustomDie(std::vector<Outcome> outcomes) : outcomes(std::move(outcomes)) {
      std::ranges::sort(this->outcomes);
    }

    constexpr auto begin() const {return outcomes.begin();}
    constexpr auto end() const{return outcomes.end();}

    template<typename PRNG>
    requires std::uniform_random_bit_generator<std::remove_reference_t<PRNG>>
    constexpr Outcome operator()(PRNG && engine) const{
      if(outcomes.size() == 0) {
	return {};
      }
      std::uniform_int_distribution<size_t> dist{0, outcomes.size() - 1};
      return outcomes.at(dist(engine));
    }

    constexpr BigInt operator()(Outcome o) const{
      return std::ranges::binary_search(outcomes, o) ? 1 : 0;
    }

    constexpr size_t size() const {return outcomes.size();}
    constexpr BigInt trials() const {return BigInt{outcomes.size()};}
  };

  static_assert(Rollable<CustomDie>);
}
