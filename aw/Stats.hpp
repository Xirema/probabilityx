#pragma once
#include <deque>
#include <expected>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "Roll.hpp"

namespace aw {
using probx::Decimal;
using probx::Integer;

struct UnitType {
  std::string name;
  std::string terrain;
  std::string commander;
  Integer copState = 0;  // 0 == d2d, 1 == COP, 2 == SCOP
  bool hasAmmo = true;
  Integer towers = 0;
  Integer properties = 0;
  Integer startingHP = 100;
};

struct UnitProperties {
  std::optional<int> baseDamage = 55;
  int firepowerBoost = 0;
  int defenseBoost = 0;
  int terrainStars = 0;
  bool damageStars = false;
  int goodLuck = 9;
  int badLuck = 0;
  int towerFirepower = 0;
  int towerDefense = 0;
  int indirectDefense = 0;
  int enemyTerrainStars = 0;

  constexpr UnitProperties addTowers(int towers) const {
    UnitProperties copy{*this};
    copy.firepowerBoost += towerFirepower * towers;
    copy.defenseBoost += towerDefense * towers;
    return copy;
  }

  constexpr UnitProperties applyStandardCOP() const {
    UnitProperties copy{*this};
    copy.firepowerBoost += 10;
    copy.defenseBoost += 10;
    return copy;
  }
  constexpr auto operator<=>(UnitProperties const& o) const = default;
};

std::expected<std::pair<UnitProperties, UnitProperties>, std::string>
getMatchup(UnitType attackingUnit, UnitType defendingUnit);
}  // namespace aw