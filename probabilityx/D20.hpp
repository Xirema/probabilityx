#pragma once
#include "Factory.hpp"
#include "compositors/Common.hpp"

namespace probx::d20 {
struct AttackRules {
  Integer attackMod{};
  std::vector<dice::RegularDie> extraAttackDice;
  std::vector<dice::RegularDie> damageDice;
  Integer damageMod{};
  Integer critRange{1};
  Integer critMultiplier{2};
  Integer extraCrit{};
  Integer extraCritFlat{};
  Integer missRange{1};
  Integer targetAC{};
  std::optional<Integer> damageRerollThreshold{};
};

namespace detail {
struct AttackRollCompositor {
  constexpr AttackRollCompositor(Integer armorClass) : armorClass(armorClass) {}
  Integer armorClass{};
  constexpr bool operator()(Outcome o) const { return o.extra <= 0; }
  constexpr Outcome operator()(Outcome attack, Outcome damage) const {
    if (attack.extra < 0 || (attack.result < armorClass && attack.extra < 1)) {
      return 0;
    }
    return damage;
  }
};
}  // namespace detail

constexpr dice::MappedRoll getAttackRoll(AttackRules rules) {
  std::ranges::sort(rules.damageDice,
                    [](dice::RegularDie const& a, dice::RegularDie const& b) {
                      return mean(a) > mean(b);
                    });
  std::vector<Outcome> d20Outcomes;
  for (Integer i = 1; i <= 20; i++) {
    d20Outcomes.push_back(Outcome{i, i <= rules.missRange          ? -1
                                     : i >= (21 - rules.critRange) ? 1
                                                                   : 0});
  }
  dice::CustomDie d20{d20Outcomes};
  dice::MappedRoll attackRoll =
      composite(compositors::adder, d20, dice::Modifier(rules.attackMod));
  for (auto const& die : rules.extraAttackDice) {
    attackRoll = composite(compositors::adder, attackRoll, die);
  }
  dice::MappedRoll damageRoll;
  dice::MappedRoll critRoll;
  if (rules.damageDice.size() > 0) {
    if (rules.damageRerollThreshold) {
      for (auto const& die : rules.damageDice) {
        dice::MappedRoll rerolledDie = composite(
            compositors::d20::rerollOnce(*rules.damageRerollThreshold), die,
            die);
        if (damageRoll.size() != 0) {
          damageRoll = composite(compositors::adder, damageRoll, rerolledDie);
        } else {
          damageRoll = rerolledDie;
        }
      }
    } else {
      damageRoll = composite(compositors::adder, rules.damageDice);
    }
    for (auto i = 1; i <= rules.critMultiplier; i++) {
      if (critRoll.size() != 0) {
        critRoll = composite(compositors::adder, critRoll, damageRoll);
      } else {
        critRoll = damageRoll;
      }
    }
    for (int i = 0; i < rules.extraCrit; i++) {
      auto const& largestDamageDie = rules.damageDice.front();
      if (rules.damageRerollThreshold) {
        auto rerolledDie = composite(
            compositors::d20::rerollOnce(*rules.damageRerollThreshold),
            largestDamageDie, largestDamageDie);
        critRoll = composite(compositors::adder, critRoll, rerolledDie);
      } else {
        critRoll = composite(compositors::adder, critRoll, largestDamageDie);
      }
    }
  }
  if (damageRoll.size() > 0) {
    damageRoll = composite(compositors::adder, damageRoll,
                           dice::Modifier{rules.damageMod});
  } else {
    damageRoll = composite(compositors::adder, dice::Modifier{rules.damageMod});
  }
  if (critRoll.size() > 0) {
    critRoll =
        composite(compositors::adder, critRoll, dice::Modifier{rules.damageMod},
                  dice::Modifier{rules.extraCritFlat});
  } else {
    critRoll = composite(compositors::adder, dice::Modifier{rules.damageMod},
                         dice::Modifier{rules.extraCritFlat});
  }

  return ternaryComposite(detail::AttackRollCompositor{rules.targetAC},
                          attackRoll, damageRoll, critRoll);
}
}  // namespace probx::d20
