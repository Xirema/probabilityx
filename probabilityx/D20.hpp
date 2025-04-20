#pragma once
#include "Factory.hpp"
#include "compositors/Common.hpp"

namespace probx::d20 {
//Rules for attackers in a generic (D&D5e.x) system
// - attackMod : Integer - Total Attack Mod (*all* modifiers combined)
// - extraAttackDice : RegularDie[] - any variable modifiers to attack rolls (bless/etc)
// - damageDice : RegularDie[] - all dice added together to calculate damage normally
// - damageMod : Integer - static damage modifier
// - critRange : Integer - default of 1, total range of critical hits (1 means only nat20 crits)
// - critMultiplier : Integer - default of 2, damage dice count multiplied by this number when crits occur
// - extraCrit : Integer - additional number of damage dice to add to crits, applied *after* multiplier
// - extraCritFlat : Integer - static damage to add to critical hits
// - missRange : Integer - default of 1, increases range of "automatic misses"
// - missDamageDice : RegularDie[] - damage that applies even on a miss
// - missDaamge : Integer - static damage that applies even on a miss
// - targetAC : Integer - determines nominal attack roll required to hit
// - damageRerollThreshold ?: Integer - determines what threshold under which damage dice may be rerolled
struct AttackRules {
  // - attackMod : Integer - Total Attack Mod (*all* modifiers combined)
  Integer attackMod{};
  // - extraAttackDice : RegularDie[] - any variable modifiers to attack rolls (bless/etc)
  std::vector<dice::RegularDie> extraAttackDice;
  // - damageDice : RegularDie[] - all dice added together to calculate damage normally
  std::vector<dice::RegularDie> damageDice;
  // - damageMod : Integer - static damage modifier
  Integer damageMod{};
  // - critRange : Integer - default of 1, total range of critical hits (1 means only nat20 crits)
  Integer critRange{1};
  // - critMultiplier : Integer - default of 2, damage dice count multiplied by this number when crits occur
  Integer critMultiplier{2};
  // - extraCrit : Integer - additional number of damage dice to add to crits, applied *after* multiplier
  Integer extraCrit{};
  // - extraCritFlat : Integer - static damage to add to critical hits
  Integer extraCritFlat{};
  // - missRange : Integer - default of 1, increases range of "automatic misses"
  Integer missRange{1};
  // - missDamageDice : RegularDie[] - damage that applies even on a miss
  std::vector<dice::RegularDie> missDamageDice;
  // - missDaamge : Integer - static damage that applies even on a miss
  Integer missDamage{};
  // - targetAC : Integer - determines nominal attack roll required to hit
  Integer targetAC{};
  // - damageRerollThreshold ?: Integer - determines what threshold under which damage dice may be rerolled
  std::optional<Integer> damageRerollThreshold{};
};

namespace detail {
//Ternary Compositor that takes an armor class as configuration
//For the boolean test, returns true if not a critical hit
//For the composite, returns 0 if the attack roll is less than the armor class, returns the damage if not
struct AttackRollCompositor {
  //Constructor
  // - armorClass : Integer - threshold that must be met for damage to be applied
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

//Coagulates a series of rolls down into a single roll representing all possible outcomes of an attack
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
