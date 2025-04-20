#include <print>

#include "aw/Calc.hpp"
#include "aw/Stats.hpp"
#include "probabilityx/Transformers.hpp"

int main() {
  auto [attacker, defender] =
      *aw::getMatchup(aw::UnitType{.name = "infantry",
                                   .terrain = "mountain",
                                   .commander = "sonja",
                                   .copState = 2,
                                   .towers = 1},
                      aw::UnitType{.name = "infantry",
                                   .terrain = "road",
                                   .commander = "rachel",
                                   .towers = 1});
  auto result = aw::calculateCombat(attacker, defender);
  auto [hpA, hpB] = probx::split(result);
  std::print("Attacker HP: \n\n");
  for (auto outcome : hpA) {
    std::print("{:} : {:%}\n", outcome, probx::oddsOf(hpA, outcome));
  }
  std::print("\nDefender HP: \n\n");
  for (auto outcome : hpB) {
    std::print("{:} : {:%}\n", outcome, probx::oddsOf(hpB, outcome));
  }
}