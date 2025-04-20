#pragma once
#include "Roll.hpp"
#include "dice/MappedRoll.hpp"

namespace probx {
//A transformer is a unary function that simply transforms one outcome into another
template <typename Func>
concept Transformer = requires(Func func, Outcome o) {
  { func(o) } -> std::convertible_to<Outcome>;
};

//Transforms the roll into a new roll that maps all the outcomes to new outcomes
constexpr dice::MappedRoll transform(Transformer auto&& transformer,
                                     Rollable auto&& roll) {
  std::map<Outcome, BigInt> map;
  for (auto outcome : roll) {
    map[transformer(outcome)] += roll(outcome);
  }
  return dice::MappedRoll{map};
}

//Convenience operator overload to apply a transformer to a roll
constexpr dice::MappedRoll operator|(Rollable auto&& roll,
                                     Transformer auto&& transformer) {
  return transform(std::forward<decltype(transformer)>(transformer),
                   std::forward<decltype(roll)>(roll));
}

//Takes a roll and splits the outcomes into two rolls
// - First Roll: the odds of each Result in the outcomes of the roll
// - Second Roll: the odds of each Extra in the outcomes of the roll
constexpr std::pair<dice::MappedRoll, dice::MappedRoll> split(
    Rollable auto&& roll) {
  auto resultOnly = [](Outcome o) { return Outcome{o.result}; };
  auto extraOnly = [](Outcome o) { return Outcome{o.extra}; };

  return split(std::forward<decltype(roll)>(roll), resultOnly, extraOnly);
}

//Takes a roll and splits the outcomes into two rolls according to the two provided transformers
constexpr std::pair<dice::MappedRoll, dice::MappedRoll> split(
    Rollable auto&& roll, Transformer auto&& resultTransformer,
    Transformer auto&& extraTransformer) {
  return std::make_pair(
      transform(resultTransformer, std::forward<decltype(roll)>(roll)),
      transform(extraTransformer, std::forward<decltype(roll)>(roll)));
}

}  // namespace probx
