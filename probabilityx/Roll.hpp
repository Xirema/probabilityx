#pragma once
#include <boost/container_hash/hash.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <compare>
#include <iterator>
#include <map>
#include <random>
#include <ranges>
#include <variant>
#include <vector>

namespace probx {
namespace types = boost::multiprecision;
//Type that represents the number of discrete trials that may yield a given result
using BigInt = types::cpp_int;
//Type that represents the odds that may yield a given result
using Rational = types::cpp_rational;
//Result Type
using Integer = std::int64_t;
//Unused
using UInteger = std::uint64_t;
//Represents odds as a floating point value
using Decimal = double;

//Contains the results of a normal die roll. 
// * result: Integer - The nominal value of an Outcome.
// * extra: Integer - Additional data associated with an Outcome. Usually equal to 0.
struct Outcome {
  //Integer - The nominal value of an Outcome
  Integer result;
  //Integer - Additional data associated with an Outcome. Usually equal to 0.
  Integer extra;

  //Default-initializes the values to 0
  constexpr Outcome() : Outcome(0) {}
  constexpr Outcome(Integer result) : Outcome(result, 0) {}
  constexpr Outcome(Integer result, Integer extra)
      : result(result), extra(extra) {}

  constexpr auto operator<=>(Outcome const&) const = default;
};

//Unused
struct ExtendedOutcome {
  std::vector<Integer> results;
};
}  // namespace probx

namespace std {
template <>
struct hash<probx::Outcome> {
  constexpr size_t operator()(probx::Outcome const& o) const {
    size_t ret = 0;
    boost::hash_combine(ret, o.result);
    boost::hash_combine(ret, o.extra);
    return ret;
  }
};
template <>
struct formatter<probx::BigInt> : formatter<string> {
  auto format(probx::BigInt const& i, format_context& ctx) const {
    stringstream ss;
    ss << i;
    return formatter<string>::format(std::format("{}", ss.str()), ctx);
  }
};
template <>
struct formatter<probx::Rational> {
  bool asPercent = false;
  constexpr auto parse(format_parse_context& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) {
      return it;
    }
    if (*it == '%') {
      asPercent = true;
      ++it;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("Invalid Format Args for Rational.");
    }
    return it;
  }

  auto format(probx::Rational const& r, format_context& ctx) const {
    stringstream ss;
    ss << r;
    probx::Decimal decimal = r.convert_to<probx::Decimal>();
    if (asPercent) {
      std::print(ss, " ({:.3f}%)", decimal * 100);
    } else {
      std::print(ss, " ({:.3f})", decimal);
    }
    return std::ranges::copy(std::move(ss).str(), ctx.out()).out;
  }
};
template <>
struct formatter<probx::Outcome> {
  bool printExtra = false;
  int width = 0;
  constexpr auto parse(format_parse_context& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) {
      return it;
    }
    std::string s;
    while (*it >= '0' && *it <= '9') {
      s.push_back(*it);
      ++it;
    }
    if (s.size() > 0) {
      width = 0;
      for (auto c : s) {
        width = width * 10 + (c - '0');
      }
    }
    if (*it == '!') {
      printExtra = true;
      ++it;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("Invalid Format Args for Outcome.");
    }
    return it;
  }
  auto format(probx::Outcome const& o, format_context& ctx) const {
    stringstream ss;
    if (printExtra) {
      std::print(ss, "[{0:{2}},{1:{2}}]", o.result, o.extra, width);
    } else {
      std::print(ss, "[{0:{1}}]", o.result, width);
    }
    return std::ranges::copy(std::move(ss).str(), ctx.out()).out;
  }
};
}  // namespace std

namespace probx {
//The root Concept of this library. A Rollable is any range of Outcomes that satisfies the following requirements:
// - Can be invoked with an outcome to yield the number of trials of the roll that yield that outcome (might be 0, should not be negative)
// - size():size_t - yields the number of unique outcomes.
// - trials():BigInt - yields the total number of trials associated with all possible outcomes
// - Can be invoked with a random number generator to yield an outcome. Should yield outcomes proportional to their odds.
//Additional Requirements (not enforced by the library):
// - When iterating over the roll, Outcomes should be yielded in ascending order according to the natural outcome ordering
template <typename Roll>
concept Rollable = std::ranges::range<Roll> &&
                   std::same_as<std::ranges::range_value_t<Roll>, Outcome> &&
                   requires(Roll roll, Outcome outcome) {
                     { roll(outcome) } -> std::convertible_to<BigInt>;
                     { roll.size() } -> std::convertible_to<size_t>;
                     { roll.trials() } -> std::convertible_to<BigInt>;
                   } && requires(Roll roll, std::minstd_rand prng) {
                     { roll(prng) } -> std::convertible_to<Outcome>;
                   };


//Given a roll, yields the odds of the provided outcome (might be 0)
constexpr Rational oddsOf(Rollable auto&& roll, Outcome o) {
  if(roll.trials() == 0) {
    return 0;
  }
  return Rational{roll(o), roll.trials()};
}
//Given a roll, yields the odds of an outcome equal to or greater than the provided outcome
constexpr Rational oddsAtLeast(Rollable auto&& roll, Outcome o) {
  Rational ret{0};
  for (auto outcome : std::views::reverse(roll)) {
    if (outcome < o) {
      break;
    }
    ret += oddsOf(roll, outcome);
  }
  return ret;
}
//Given a roll, yields the odds of an outcome strictly greater than the provided outcome
constexpr Rational oddsMoreThan(Rollable auto&& roll, Outcome o) {
  Rational ret{0};
  for (auto outcome : std::views::reverse(roll)) {
    if (outcome <= o) {
      break;
    }
    ret += oddsOf(roll, outcome);
  }
  return ret;
}
//Given a roll, yields the odds of an outcome equal to or less than the provided outcome
constexpr Rational oddsAtMost(Rollable auto&& roll, Outcome o) {
  Rational ret{0};
  for (auto outcome : roll) {
    if (outcome > o) {
      break;
    }
    ret += oddsOf(roll, outcome);
  }
  return ret;
}
//Given a roll, yields the odds of an outcome strictly less than the provided outcome
constexpr Rational oddsLessThan(Rollable auto&& roll, Outcome o) {
  Rational ret{0};
  for (auto outcome : roll) {
    if (outcome >= o) {
      break;
    }
    ret += oddsOf(roll, outcome);
  }
  return ret;
}
//Calculates the average result of this roll's outcomes. 
//Retrieving the average extra should be obtained from a transformation that filters the extra value.
constexpr Decimal mean(Rollable auto&& roll) {
  Rational ret{};
  for (auto outcome : roll) {
    ret += outcome.result * oddsOf(roll, outcome);
  }
  return ret.convert_to<Decimal>();
}

//Calculates which outcome represents the provided percentile of the total distribution
//(work in progress)
constexpr Outcome percentile(Rollable auto&& roll, Decimal percent) {
  Rational odds{};
  if(percent < 0 || percent > 1) {
    return {};
  }
  for (auto outcome : roll) {
    odds += oddsOf(roll, outcome);
    if (odds >= percent) {
      return outcome;
    }
  }
  return {};
}

//Calculates the median outcome of a roll
//Equivalent to percentile(0.5)
constexpr Outcome median(Rollable auto&& roll) {
  return percentile(std::forward<decltype(roll)>(roll), 0.5);
}

//Calculates the mode outcome of a roll
constexpr Outcome mode(Rollable auto&& roll) {
  Rational odds{};
  Outcome o{};
  for (auto outcome : roll) {
    auto currOdds = oddsOf(roll, outcome);
    if (currOdds > odds) {
      odds = currOdds;
      o = outcome;
    }
  }
  return o;
}

//Calculates the minimum outcome of a roll
constexpr Outcome min(Rollable auto&& roll) {
  for (auto outcome : roll) {
    return outcome;
  }
  return {};
}

//Calculates the maximum outcome of a roll
constexpr Outcome max(Rollable auto&& roll) {
  for (auto outcome : std::views::reverse(roll)) {
    return outcome;
  }
  return {};
}

//Convenience method for calculating the hash of an arbitrary roll
constexpr size_t hash(Rollable auto&& roll) {
  size_t ret{};
  for (Outcome o : roll) {
    boost::hash_combine(ret, o);
    boost::hash_combine(ret, roll(o));
  }
  return ret;
}

//Checks if a heterogeneous pair of rolls represents the same distribution
constexpr bool equals(Rollable auto&& a, Rollable auto&& b) {
  if (a.size() != b.size()) {
    return false;
  }
  auto zipped_view = std::ranges::zip_view(a, b);
  return !std::ranges::any_of(zipped_view, [&a, &b](auto&& tuple) {
    return std::get<0>(tuple) != std::get<1>(tuple) ||
           oddsOf(a, std::get<0>(tuple)) != oddsOf(b, std::get<1>(tuple));
  });
}
}  // namespace probx
