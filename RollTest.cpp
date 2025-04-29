#include <print>
#include <random>
#include <ranges>

#include "probabilityx/Compositors.hpp"
#include "probabilityx/D20.hpp"
#include "probabilityx/Factory.hpp"
#include "probabilityx/Roll.hpp"
#include "probabilityx/compositors/Common.hpp"
#include "probabilityx/dice/MappedRoll.hpp"
#include "probabilityx/dice/Modifier.hpp"
#include "probabilityx/dice/RegularDie.hpp"
#include "probabilityx/dice/XdYRoll.hpp"

int main() {
  probx::dice::RegularDie die{6};
  auto printRoll = [](probx::Rollable auto&& roll) {
    for (auto outcome : roll) {
      std::print(std::cout, "Odds of {}: {:%}\n", outcome,
                 oddsOf(roll, outcome));
    }
  };
  printRoll(die);

  std::print(std::cout, "Odds of at least rolling a 3: {:%}\n",
             probx::oddsAtLeast(die, 3));
  std::print(std::cout, "Odds of at most rolling a 2: {:%}\n",
             probx::oddsAtMost(die, 2));
  std::print(std::cout, "Odds of rolling less than 5: {:%}\n",
             probx::oddsLessThan(die, 5));
  std::print(std::cout, "Odds of rolling more than 4: {:%}\n",
             probx::oddsMoreThan(die, 4));
  probx::dice::Modifier mod{3};
  printRoll(mod);

  std::print(std::cout, "Mean of Regular Die: {:.3f}\n", probx::mean(die));
  std::print(std::cout, "Median of Regular Die: {}\n", probx::median(die));
  std::print(std::cout, "Max of Modifier: {}\n", probx::max(mod));

  std::map<probx::Outcome, probx::BigInt> map;
  map[1] = 1;
  map[2] = 1;
  map[3] = 1;
  map[4] = 7;
  probx::dice::MappedRoll mapped{map};
  printRoll(mapped);
  std::minstd_rand engine{std::random_device{}()};
  static_assert(std::uniform_random_bit_generator<std::minstd_rand>);
  std::print(std::cout, "Randomly generated outcome: {}\n", mapped(engine));

  using probx::compositors::adder;
  auto twoDSix =
      probx::composite(adder, die, die, die, probx::dice::Modifier(5));

  printRoll(twoDSix);

  std::vector<probx::dice::RegularDie> dice;
  dice.emplace_back(4);
  dice.emplace_back(6);
  auto d4d6 = probx::composite(adder, dice);

  printRoll(d4d6);

  dice = {};
  for (int i = 0; i < 4; i++) {
    dice.emplace_back(2);
  }

  auto fourDTwo = probx::composite(adder, dice);

  printRoll(fourDTwo);

  probx::RollFactory factory{};

  std::visit(printRoll, factory.getXdYRoll(2, 6, 2));

  std::visit(printRoll, factory.getXdYRoll(1, 6, 2));

  auto d5OddD7Even = probx::ternaryComposite(
      probx::compositors::evenodd{}, probx::dice::RegularDie{6},
      probx::dice::RegularDie{5}, probx::dice::RegularDie{7});
  printRoll(d5OddD7Even);

  namespace d20 = probx::d20;
  d20::AttackRules attackRules;
  attackRules.attackMod = 5;
  attackRules.damageDice = {probx::dice::RegularDie{6},
                            probx::dice::RegularDie{6}};
  attackRules.damageMod = 3;
  auto l1Greatsword = d20::getAttackRoll(attackRules);
  std::print(std::cout, "\nNow Testing a Level 1 Greatsword!\n\n");
  printRoll(l1Greatsword);
  std::print(std::cout, "\n=== Mean: {:.3f}\n\n", probx::mean(l1Greatsword));

  attackRules.damageRerollThreshold = 2;
  auto l1GreatswordGwf = d20::getAttackRoll(attackRules);
  printRoll(l1GreatswordGwf);
  std::print(std::cout, "\n=== Mean: {:.3f}\n", probx::mean(l1GreatswordGwf));

  attackRules.extraAttackDice = {probx::dice::RegularDie{4}};

  probx::dice::XdYRoll native3D6{3, 6};
  auto factory3D6 = factory.getXdYRoll(3, 6);
  printRoll(native3D6);
  std::visit(printRoll, factory3D6);

  std::print("\n");
  /*std::map<probx::Outcome, int> counts;
  for(int i = 0; i < 1'000'000; i++) {
    counts[native3D6(engine)]++;
  }
  for(auto [outcome, count] : counts) {
    std::print("{:2} : {:5} ({:7.3f}%)\n", outcome, count, count / 10'000.00);
    }*/

  // probx::dice::XdYRoll native400D20{400, 20};
  // printRoll(native400D20);

  probx::dice::RegularDie d6{6};
  using namespace probx::compositors;
  auto positiveTwoDSix = d6 + d6;
  printRoll(positiveTwoDSix);
  auto negativeTwoDSix = d6 - d6;
  printRoll(negativeTwoDSix);

  probx::dice::MappedRoll::Map critFail;
  critFail[1] = 1;
  critFail[0] = 19;

  probx::dice::MappedRoll critFailRoll{critFail};
  std::vector<probx::dice::MappedRoll> vecOfRolls;
  vecOfRolls.resize(168, critFailRoll);
  // auto finalRoll = critFailRoll;
  // for(int i = 1; i < 168; i++) {
  //   finalRoll = finalRoll + critFailRoll;
  // }
  auto finalRoll = probx::composite(probx::compositors::adder, vecOfRolls);
  std::println("");
  std::println("Final Roll:");
  printRoll(finalRoll);
}
