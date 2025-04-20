#pragma once
#include <print>

#include "Compositors.hpp"
#include "Stats.hpp"
#include "compositors/Common.hpp"
#include "dice/Dice.hpp"

namespace aw {
constexpr Integer awround(Decimal f) {
  f = std::round(f);
  return static_cast<Integer>(f);
}

constexpr Integer awround2(Decimal f) {
  Decimal fract = f - std::floor(f);
  if (fract >= 0.95)
    return static_cast<Integer>(std::ceil(f));
  else
    return static_cast<Integer>(std::floor(f));
}

constexpr Integer effectiveHealth(Integer health) {
  auto healthF = health / 10.;
  healthF = std::ceil(healthF);
  return static_cast<Integer>(healthF) * 10;
}

namespace compositors {
constexpr Integer MINHP = 0;
constexpr Integer MAXHP = 100;
struct CombatCompositor {
  UnitProperties unitA{};
  UnitProperties unitB{};
  constexpr probx::Outcome operator()(probx::Outcome a,
                                      probx::Outcome b) const {
    if (!unitA.baseDamage) {
      // Literally nothing happens.
      return probx::Outcome(a.extra, b.extra);
    }
    double unitDamageA = *unitA.baseDamage;
    double firepowerBoostA =
        100 + unitA.firepowerBoost +
        (unitA.damageStars ? unitA.terrainStars
                           : 0)  //* effectiveHealth(a.special) / 10.
        ;
    firepowerBoostA = std::max(firepowerBoostA, 0.);
    unitDamageA *= firepowerBoostA / 100;

    // Applies Luck
    unitDamageA += a.result;

    // Reduces damage based on current health
    unitDamageA *= effectiveHealth(a.extra) / 100.;

    // Correct for negative damage
    // Used in cartridge, not used in AWBW
    // unitDamageA = std::max(unitDamageA, 0.);

    double defenseBoostB = 100 + unitB.defenseBoost +
                           unitB.terrainStars * effectiveHealth(b.extra) / 10.;

    // Applies Defense
    unitDamageA *= (200 - defenseBoostB) / 100;

    // Clamps to prevent negative damage
    unitDamageA = std::max(unitDamageA, 0.);

    int finalDamageA = awround2(unitDamageA);

    int newHealthB = std::clamp(b.extra - finalDamageA, MINHP, MAXHP);

    if (!unitB.baseDamage) {
      // Defending Unit cannot return fire
      return probx::Outcome{a.extra, newHealthB};
    }
    double unitDamageB = *unitB.baseDamage;
    double firepowerBoostB = 100 + unitB.firepowerBoost +
                             (unitB.damageStars ? unitB.terrainStars : 0) *
                                 effectiveHealth(newHealthB) / 10.;
    firepowerBoostB = std::max(firepowerBoostB, 0.);
    unitDamageB *= firepowerBoostB / 100;

    // Applies Luck
    unitDamageB += b.result;

    // Reduces damage based on current health
    unitDamageB *= effectiveHealth(newHealthB) / 100.;

    // Correct for negative damage
    // Used in cartridge, not used in AWBW
    // unitDamageB = std::max(unitDamageB, 0.);

    double defenseBoostA = 100 + unitA.defenseBoost +
                           unitA.terrainStars * effectiveHealth(a.extra) / 10.;

    // Applies Defense
    unitDamageB *= (200 - defenseBoostA) / 100;

    // Clamps to prevent negative damage
    unitDamageB = std::max(unitDamageB, 0.);

    int finalDamageB = awround2(unitDamageB);

    int newHealthA = std::clamp(a.extra - finalDamageB, MINHP, MAXHP);
    return probx::Outcome{newHealthA, newHealthB};
  }
};

constexpr probx::Outcome fuse(probx::Outcome a, probx::Outcome b) {
  return probx::Outcome{a.result, b.result};
}
}  // namespace compositors

constexpr probx::dice::MappedRoll calculateCombat(
    UnitProperties const &attacker, UnitProperties const &defender,
    probx::Rollable auto &&attackerHP, probx::Rollable auto &&defenderHP) {
  compositors::CombatCompositor compositor{.unitA = attacker,
                                           .unitB = defender};
  probx::dice::MappedRoll attackerLuck = probx::dice::Modifier{0};
  if (attacker.goodLuck > 0) {
    attackerLuck =
        probx::composite(probx::compositors::adder, attackerLuck,
                         probx::dice::RegularDie{0, attacker.goodLuck});
  }
  if (attacker.badLuck > 0) {
    attackerLuck =
        probx::composite(probx::compositors::adder, attackerLuck,
                         probx::dice::RegularDie{-attacker.badLuck, 0});
  }

  probx::dice::MappedRoll defenderLuck = probx::dice::Modifier{0};
  if (defender.goodLuck > 0) {
    defenderLuck =
        probx::composite(probx::compositors::adder, defenderLuck,
                         probx::dice::RegularDie{0, defender.goodLuck});
  }
  if (defender.badLuck > 0) {
    defenderLuck =
        probx::composite(probx::compositors::adder, defenderLuck,
                         probx::dice::RegularDie{-defender.badLuck, 0});
  }

  auto unitAttacker =
      probx::composite(compositors::fuse, attackerLuck, attackerHP);
  auto unitDefender =
      probx::composite(compositors::fuse, defenderLuck, defenderHP);

  return probx::composite(compositor, unitAttacker, unitDefender);
}

constexpr probx::dice::MappedRoll calculateCombat(
    UnitProperties const &attacker, UnitProperties const &defender) {
  return calculateCombat(attacker, defender, probx::dice::Modifier{100},
                         probx::dice::Modifier{100});
}
}  // namespace aw