#pragma once
#include <deque>

#include "Roll.hpp"
#include "dice/Dice.hpp"

namespace probx {

template <typename Comp>
//Compositors are a functor that can take a pair of outcomes and transform them into another outcome
//The simplest compositors can merely add or subtract two outcomes
//The only requirement is that the Compositor be invokable using two outcomes to produce an outcome
concept Compositor = requires(Comp comp, Outcome o) {
  { comp(o, o) } -> std::convertible_to<Outcome>;
};

//Used for factory functions that cannot know at compile time what kind of die will be returned
//Variant of
// - RegularDie
// - CustomDie
// - Modifier
// - MappedRoll
using RollVariant = std::variant<dice::RegularDie, dice::CustomDie,
                                 dice::Modifier, dice::MappedRoll>;

namespace detail {
using OutcomeStack = std::deque<std::pair<Outcome, BigInt>>;
constexpr void composite_impl(dice::MappedRoll::Map& map,
                              OutcomeStack& outcomeStack,
                              Compositor auto&& func) {
  if (outcomeStack.size() == 0) {
    return;
  }
  if (outcomeStack.size() == 1) {
    map[outcomeStack.front().first] += outcomeStack.front().second;
    return;
  }
  OutcomeStack ret;
  std::partial_sum(
      outcomeStack.begin(), outcomeStack.end(), std::back_inserter(ret),
      [&func](std::pair<Outcome, BigInt> const& a,
              std::pair<Outcome, BigInt> const& b) {
        return std::make_pair(func(a.first, b.first), a.second * b.second);
      });
  auto [outcome, trials] = ret.back();
  map[outcome] += trials;
}

constexpr void composite_impl(dice::MappedRoll::Map& map,
                              OutcomeStack& outcomeStack,
                              Compositor auto&& func, Rollable auto&& roll,
                              Rollable auto&&... rolls) {
  for (auto outcome : roll) {
    outcomeStack.push_back(std::make_pair(outcome, roll(outcome)));
    composite_impl(map, outcomeStack, std::forward<decltype(func)>(func),
                   std::forward<decltype(rolls)>(rolls)...);
    outcomeStack.pop_back();
  }
}
}  // namespace detail

//Take an arbitrary number of rolls and composite them together
//For a given list of rolls, compositing happens left-to-right
//So for [a, b, c, d], the order is composite(composite(composite(a,b), c), d)
constexpr dice::MappedRoll composite(Compositor auto&& func,
                                     Rollable auto&&... rolls) {
  detail::OutcomeStack outcomeStack;
  dice::MappedRoll::Map map;
  detail::composite_impl(map, outcomeStack, std::forward<decltype(func)>(func),
                         std::forward<decltype(rolls)>(rolls)...);
  return dice::MappedRoll{map};
}

//Composites a range of rolls
//For large ranges the range is subdivided in half and recursively composited
template <std::ranges::range Rolls>
  requires Rollable<std::ranges::range_value_t<Rolls>>
constexpr dice::MappedRoll composite(Compositor auto&& func, Rolls&& rolls) {
  // static_assert(Rollable<std::ranges::range_value_t<decltype(rolls)>>);
  size_t size = std::ranges::size(rolls);
  if (size == 0) {
    return {};
  } else if (size == 1) {
    return composite(std::forward<decltype(func)>(func),
                     *std::ranges::begin(rolls));
  } else if (size == 2) {
    auto r1 = std::ranges::begin(rolls);
    auto r2 = r1;
    ++r2;
    return composite(std::forward<decltype(func)>(func), *r1, *r2);
  } else {
    auto chunks = std::ranges::chunk_view(rolls, size / 2);
    auto mappedRoll1 = composite(std::forward<decltype(func)>(func),
                                 *std::ranges::begin(chunks));
    auto mappedRoll2 = composite(std::forward<decltype(func)>(func),
                                 *std::ranges::rbegin(chunks));
    return composite(std::forward<decltype(func)>(func), mappedRoll1,
                     mappedRoll2);
  }
}

//A Ternary Compositor has two functions:
// - invoked with two outcomes to produce one new outcome
// - invoked with one outcome to produce a bool
template <typename Func>
concept TernaryCompositor = requires(Func func, Outcome o) {
  { func(o, o) } -> std::convertible_to<Outcome>;
  { func(o) } -> std::convertible_to<bool>;
};

//A ternary composite takes in three rolls.
// - For each outcome of the decision roll, the "test" of the compositor is invoked
// - If the test is "true", then the decision outcome is composited with all outcomes of the main roll
// - If the test is "false", then the decision outcome is composited with all the outcomes of the alternate roll
constexpr dice::MappedRoll ternaryComposite(TernaryCompositor auto&& func,
                                            Rollable auto&& decisionRoll,
                                            Rollable auto&& mainRoll,
                                            Rollable auto&& alternateRoll) {
  std::map<Outcome, Rational> tempMap;
  for (auto outcome : decisionRoll) {
    Rational odds = oddsOf(decisionRoll, outcome);
    bool test = func(outcome);
    auto loopFunc = [outcome, &odds, &tempMap, &func](Rollable auto&& roll) {
      for (auto secondOutcome : roll) {
        Outcome finalOutcome = func(outcome, secondOutcome);
        tempMap[finalOutcome] += odds * oddsOf(roll, secondOutcome);
      }
    };

    if (test) {
      loopFunc(mainRoll);
    } else {
      loopFunc(alternateRoll);
    }
  }

  std::map<Outcome, BigInt> finalMap;
  BigInt lcd = 1;
  for (auto const& [outcome, odds] : tempMap) {
    lcd = lcm(lcd, denominator(odds));
  }
  for (auto const& [outcome, odds] : tempMap) {
    finalMap[outcome] = (odds * lcd).convert_to<BigInt>();
  }
  return dice::MappedRoll{finalMap};
}
}  // namespace probx
