#pragma once
#include "../Compositors.hpp"

namespace probx::compositors {
  constexpr Outcome adder(Outcome a, Outcome b) {return Outcome{a.result + b.result, a.extra + b.extra};}
  constexpr Outcome advantage(Outcome a, Outcome b) {return a > b ? a : b;}
  constexpr Outcome disadvantage(Outcome a, Outcome b) {return a < b ? a : b;}
  struct evenodd {
    constexpr bool operator()(Outcome o) const {
      return o.result % 2 == 1;
    }
    constexpr Outcome operator()(Outcome test, Outcome o) const {
      return o;
    }
  };
  namespace d20 {
    constexpr Outcome advantage(Outcome a, Outcome b) {
      if(a.extra != b.extra) {
	return a.extra > b.extra ? a : b;
      }
      return probx::compositors::advantage(a, b);
    }
    constexpr Outcome disadvantage(Outcome a, Outcome b) {
      if(a.extra != b.extra) {
	return a.extra < b.extra ? a : b;
      }
      return probx::compositors::disadvantage(a, b);
    }
    constexpr auto rerollOnce(Outcome threshold) {
      return [threshold](Outcome a, Outcome b) {
	return a > threshold ? a : b;
      };
    }
  }
}
