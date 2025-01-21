#pragma once
#include "Roll.hpp"
#include "dice/MappedRoll.hpp"

namespace probx {
  template<typename Func>
  concept Transformer = requires(Func func, Outcome o) {
    {func(o) } -> std::convertible_to<Outcome>;
  };

  constexpr dice::MappedRoll transform
  (
   Transformer auto && transformer,
   Rollable auto && roll
   ) {
    std::map<Outcome, BigInt> map;
    for(auto outcome : roll) {
      map[transformer(outcome)] += roll(outcome);
    }
    return dice::MappedRoll{map};
  }

  constexpr std::pair<dice::MappedRoll, dice::MappedRoll> split(Rollable auto && roll) {
    auto resultOnly = [](Outcome o) {return Outcome{o.result};};
    auto extraOnly = [](Outcome o) {return Outcome{o.extra};};

    return std::make_pair
      (
       transform(resultOnly, roll),
       transform(extraOnly, roll)
       );
  }
			  
}
