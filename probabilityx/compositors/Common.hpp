#pragma once
#include "../Compositors.hpp"
#include "../Transformers.hpp"

namespace probx::compositors {
constexpr Outcome adder(Outcome a, Outcome b) {
  return Outcome{a.result + b.result, a.extra + b.extra};
}
constexpr Outcome advantage(Outcome a, Outcome b) { return a > b ? a : b; }
constexpr Outcome disadvantage(Outcome a, Outcome b) { return a < b ? a : b; }
struct evenodd {
  constexpr bool operator()(Outcome o) const { return o.result % 2 == 1; }
  constexpr Outcome operator()(Outcome test, Outcome o) const { return o; }
};
constexpr dice::MappedRoll operator+(Rollable auto&& a, Rollable auto&& b) {
  return composite(adder, a, b);
}
constexpr dice::MappedRoll operator-(Rollable auto&& a, Rollable auto&& b) {
  return a + (b | [](Outcome o) { return Outcome{-o.result, -o.extra}; });
}
namespace d20 {
constexpr Outcome advantage(Outcome a, Outcome b) {
  if (a.extra != b.extra) {
    return a.extra > b.extra ? a : b;
  }
  return probx::compositors::advantage(a, b);
}
constexpr Outcome disadvantage(Outcome a, Outcome b) {
  if (a.extra != b.extra) {
    return a.extra < b.extra ? a : b;
  }
  return probx::compositors::disadvantage(a, b);
}
constexpr auto rerollOnce(Outcome threshold) {
  return [threshold](Outcome a, Outcome b) { return a > threshold ? a : b; };
}
}  // namespace d20
}  // namespace probx::compositors
