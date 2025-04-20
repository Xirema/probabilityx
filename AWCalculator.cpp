#include <boost/algorithm/string.hpp>
#include <expected>
#include <print>

#include "Transformers.hpp"
#include "aw/Calc.hpp"
#include "aw/Stats.hpp"

using namespace aw;

enum class ParseError { incorrectCount, invalidNumber };

struct UnitData : UnitType {
  probx::dice::MappedRoll hitPoints{probx::dice::Modifier{100}};
  Integer firepowerMod{0};
  Integer defenseMod{0};
};

constexpr std::expected<UnitData, ParseError> parse(std::string_view string) {
  std::vector<std::string_view> parts;
  boost::split(parts, string, [](char c) { return c == '|'; });
  if (parts.size() != 10) {
    return std::unexpected(ParseError::incorrectCount);
  }
  UnitData ret;
  ret.name = std::string{parts.at(0)};
  ret.terrain = std::string{parts.at(1)};
  ret.commander = std::string{parts.at(2)};
  try {
    if (auto copState = std::string{parts.at(3)}; copState.size() > 0) {
      ret.copState = std::stoi(copState);
    }
    if (parts.at(4) == "false") {
      ret.hasAmmo = false;
    }
    if (auto towers = std::string{parts.at(5)}; towers.size() > 0) {
      ret.towers = std::stoi(towers);
    }
    if (auto properties = std::string{parts.at(6)}; properties.size() > 0) {
      ret.properties = std::stoi(properties);
    }
    if (auto startingHP = std::string{parts.at(7)}; startingHP.size() > 0) {
      std::vector<std::string_view> hpparts;
      boost::split(hpparts, startingHP, [](char c) { return c == ','; });
      // ret.startingHP = std::stoi(startingHP);
      probx::dice::MappedRoll::Map map{};
      for (auto hp : hpparts) {
        ++map[std::stoi(std::string(hp))];
      }
      ret.hitPoints = map;
      ret.startingHP = static_cast<Integer>(probx::mean(ret.hitPoints));
    }
    if (auto firepowerMod = std::string{parts.at(8)}; firepowerMod.size() > 0) {
      ret.firepowerMod = std::stoi(firepowerMod);
    }
    if (auto defenseMod = std::string{parts.at(9)}; defenseMod.size() > 0) {
      ret.defenseMod = std::stoi(defenseMod);
    }
    return ret;
  } catch (...) {
    return std::unexpected(ParseError::invalidNumber);
  }
}

int main() {
  std::map<std::string, UnitData> units;
  std::string line;
  int lineNumber = 0;
  auto clampHitpoints = [](probx::Outcome o) {
    return probx::Outcome{o.result > 100 ? 100 : o.result < 0 ? 0 : o.result};
  };
  while (std::getline(std::cin, line)) {
    ++lineNumber;
    if (line.size() > 0 && line.at(0) == '#') {
      continue;
    }
    std::vector<std::string_view> args;
    if (line.find(':') != std::string::npos) {
      boost::split(args, line, [](char c) { return c == ':'; });
      if (args.size() != 2) {
        std::print(std::cerr,
                   "Improperly specified Unit Definition on line {}\n",
                   lineNumber);
        return -1;
      }
      // Defining a new unit
      auto& unitData = units[std::string{args.at(0)}];
      if (auto parsed = parse(args.at(1)); parsed.has_value()) {
        unitData = *parsed;
        unitData.hitPoints =
            probx::transform(clampHitpoints, unitData.hitPoints);
      } else {
        if (parsed.error() == ParseError::invalidNumber) {
          std::print(std::cerr, "Unable to parse number in line {}: '{}'\n",
                     lineNumber, line);
        } else if (parsed.error() == ParseError::incorrectCount) {
          std::print(std::cerr, "Incorrect Argument Count in line {}: '{}'\n",
                     lineNumber, line);
        }
        return -1;
      }
    } else if (line.find('>') != std::string::npos) {
      boost::split(args, line, [](char c) { return c == '>'; });
      if (args.size() != 2) {
        std::print(std::cerr, "Improperly specified Attack on line {}\n",
                   lineNumber);
        return -1;
      }
      std::string attackerId{args.at(0)};
      std::string defenderId{args.at(1)};
      if (units.find(attackerId) == units.end() ||
          units.find(defenderId) == units.end()) {
        std::print(std::cerr, "Invalid unit Id specified on line {}: {}, {}\n",
                   lineNumber, attackerId, defenderId);
        return -1;
      }
      auto& attacker = units.at(attackerId);
      auto& defender = units.at(defenderId);
      if (auto propertiesPair = getMatchup(attacker, defender);
          propertiesPair.has_value()) {
        auto& [attackProps, defendProps] = *propertiesPair;
        attackProps.firepowerBoost += attacker.firepowerMod;
        attackProps.defenseBoost += attacker.defenseMod;
        defendProps.firepowerBoost += defender.firepowerMod;
        defendProps.defenseBoost += defender.defenseMod;
        auto results = calculateCombat(attackProps, defendProps,
                                       attacker.hitPoints, defender.hitPoints);
        std::tie(attacker.hitPoints, defender.hitPoints) =
            probx::split(results);
      } else {
        std::print(std::cerr,
                   "Units in attack on line {} are specified improperly: {}\n",
                   lineNumber, propertiesPair.error());
        return -1;
      }
    } else if (line.find('!') != std::string::npos) {
      boost::split(args, line, [](char c) { return c == '!'; });
      if (args.size() != 2) {
        std::print(std::cerr, "Improperly specified Command on line {}\n",
                   lineNumber);
        return -1;
      }

      std::string command{args.at(0)};
      std::string commandArgs{args.at(1)};
      if (command == "print") {
        std::string unitName = std::move(commandArgs);
        auto unitIt = units.find(unitName);
        if (unitIt == units.end()) {
          std::print(std::cerr, "Unable to find unit '{}'\n", unitName);
          return -1;
        }
        auto const& [unitId, unitData] = *unitIt;
        std::print(
            "{}: {} on {} controlled by {}, in state {} with {}ammo, with {} "
            "towers and {} properties, at (approximately) {:.0f} Hitpoints.\n",
            unitId, unitData.name, unitData.terrain, unitData.commander,
            unitData.copState, unitData.hasAmmo ? "" : "no ", unitData.towers,
            unitData.properties, probx::mean(unitData.hitPoints));
        for (auto hp : unitData.hitPoints) {
          std::print(
              "  {:3}: {:7.3f}%\n", hp,
              probx::oddsOf(unitData.hitPoints, hp).convert_to<Decimal>() *
                  100);
        }
      } else if (command == "echo") {
        std::print("{}\n", commandArgs);
      }
    } else if (line.find('<') != std::string::npos) {
      boost::split(args, line, [](char c) { return c == '<'; });
      if (args.size() != 3) {
        std::print(
            std::cerr,
            "Incorrect number of arguments for Unit Modification on line {}.\n",
            lineNumber);
        return -1;
      }
      std::string unitId{args.at(0)};
      std::string_view type = args.at(1);
      std::string change{args.at(2)};
      if (units.find(unitId) == units.end()) {
        std::print(std::cerr, "Unable to find unit '{}' on line {}.\n", unitId,
                   lineNumber);
        return -1;
      }
      auto& unitData = units.at(unitId);
      try {
        if (type == "terrain") {
          unitData.terrain = change;
        } else if (type == "copstate") {
          unitData.copState = std::stoi(change);
        } else if (type == "ammo") {
          unitData.hasAmmo = change == "false" ? false : true;
        } else if (type == "towers") {
          unitData.towers = std::stoi(change);
        } else if (type == "properties") {
          unitData.properties = std::stoi(change);
        } else if (type == "damage") {
          unitData.hitPoints =
              probx::composite(probx::compositors::adder, unitData.hitPoints,
                               probx::dice::Modifier{-std::stoi(change)});
          unitData.hitPoints =
              probx::transform(clampHitpoints, unitData.hitPoints);
        } else if (type == "heal") {
          unitData.hitPoints =
              probx::composite(probx::compositors::adder, unitData.hitPoints,
                               probx::dice::Modifier{std::stoi(change)});
          unitData.hitPoints =
              probx::transform(clampHitpoints, unitData.hitPoints);
        } else if (type == "roundup") {
          unitData.hitPoints = probx::transform(
              [](probx::Outcome o) {
                return probx::Outcome{
                    static_cast<Integer>(std::ceil(o.result / 10.)) * 10};
              },
              unitData.hitPoints);
          unitData.hitPoints =
              probx::transform(clampHitpoints, unitData.hitPoints);
        }
      } catch (...) {
        std::print(std::cerr, "Error parsing Unit Modification on line {}.\n",
                   lineNumber);
        return -1;
      }
    }
  }
}