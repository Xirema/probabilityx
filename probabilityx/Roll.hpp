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
using BigInt = types::cpp_int;
using Rational = types::cpp_rational;
using Integer = std::int64_t;
using UInteger = std::uint64_t;
using Decimal = double;

struct Outcome {
  Integer result;
  Integer extra;

  constexpr Outcome() : Outcome(0) {}
  constexpr Outcome(Integer result) : Outcome(result, 0) {}
  constexpr Outcome(Integer result, Integer extra)
      : result(result), extra(extra) {}

  constexpr auto operator<=>(Outcome const&) const = default;
};
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
template <typename Roll>
concept Rollable = std::ranges::range<Roll> &&
                   std::same_as<std::ranges::range_value_t<Roll>, Outcome> &&
                   requires(Roll roll, Outcome outcome) {
                     //{ roll(prng) } -> std::convertible_to<Outcome>;
                     { roll(outcome) } -> std::convertible_to<BigInt>;
                     { roll.size() } -> std::convertible_to<size_t>;
                     { roll.trials() } -> std::convertible_to<BigInt>;
                   } && requires(Roll roll, std::minstd_rand prng) {
                     { roll(prng) } -> std::convertible_to<Outcome>;
                   };

// constexpr auto reify(Rollable auto && roll) -> decltype(roll) {return roll;}

constexpr Rational oddsOf(Rollable auto&& roll, Outcome o) {
  return Rational{roll(o), roll.trials()};
}

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

constexpr Decimal mean(Rollable auto&& roll) {
  Rational ret{};
  for (auto outcome : roll) {
    ret += outcome.result * oddsOf(roll, outcome);
  }
  return ret.convert_to<Decimal>();
}

constexpr Outcome percentile(Rollable auto&& roll, Decimal percent) {
  Rational odds{};
  for (auto outcome : roll) {
    odds += oddsOf(roll, outcome);
    if (odds >= percent) {
      return outcome;
    }
  }
  // This should never happen!
  return {};
}

constexpr Outcome median(Rollable auto&& roll) {
  return percentile(std::forward<decltype(roll)>(roll), 0.5);
}

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

constexpr Outcome min(Rollable auto&& roll) {
  for (auto outcome : roll) {
    return outcome;
  }
  return {};
}

constexpr Outcome max(Rollable auto&& roll) {
  for (auto outcome : std::views::reverse(roll)) {
    return outcome;
  }
  return {};
}

constexpr size_t hash(Rollable auto&& roll) {
  size_t ret{};
  for (Outcome o : roll) {
    boost::hash_combine(ret, o);
    boost::hash_combine(ret, roll(o));
  }
  return ret;
}

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
