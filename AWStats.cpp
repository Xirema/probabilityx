#include "aw/Stats.hpp"

std::expected<std::pair<aw::UnitProperties, aw::UnitProperties>, std::string> aw::getMatchup(aw::UnitType attackingUnit, aw::UnitType defendingUnit) {
  std::map<std::string, int> terrainStars;
  terrainStars["road"] = 0;
  terrainStars["plains"] = 1;
  terrainStars["forest"] = 2;
  terrainStars["mountain"] = 4;
  terrainStars["silo"] = 3;
  terrainStars["silo_flat"] = 3;
  terrainStars["city"] = 3;
  terrainStars["base"] = 3;
  terrainStars["airport"] = 3;
  terrainStars["port"] = 3;
  terrainStars["headquarters"] = 4;
  terrainStars["shoals"] = 0;
  terrainStars["river"] = 0;
  terrainStars["reef"] = 2;
  terrainStars["sea"] = 0;
  terrainStars["bridge"] = 0;
  terrainStars["pipeline"] = 0;
  terrainStars["pipeline_seam"] = 0;
  terrainStars["pipeline_seam_broken"] = 0;
  terrainStars["lab"] = 3;
  terrainStars["tower"] = 3;
  std::map<std::string, std::map<std::string, std::pair<std::optional<int>, std::optional<int>>>> unitMatchups;
  struct UnitData {
    std::deque<bool> weaponRequiresAmmo;
    std::vector<std::string> classifications;
  };
  std::map<std::string, UnitData> unitData;

  {
    auto& newUnitData = unitData["infantry"];
    newUnitData.classifications.push_back("footsoldier");
    newUnitData.classifications.push_back("land");
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["mech"];
    newUnitData.classifications.push_back("footsoldier");
    newUnitData.classifications.push_back("land");
    newUnitData.weaponRequiresAmmo.push_back(true);
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["recon"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["apc"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("transport");
    newUnitData.classifications.push_back("non_combat");
    newUnitData.classifications.push_back("transport_infantry");
    newUnitData.classifications.push_back("supply");
    newUnitData.classifications.push_back("auto_supply");
    newUnitData.classifications.push_back("supply_area");
  }
  {
    auto& newUnitData = unitData["tank"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["anti_air"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["artillery"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("indirect");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["medium_tank"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["neo_tank"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["rocket"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("indirect");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["missile"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("indirect");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["mega_tank"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["piperunner"];
    newUnitData.classifications.push_back("vehicle");
    newUnitData.classifications.push_back("land");
    newUnitData.classifications.push_back("indirect");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["transport_copter"];
    newUnitData.classifications.push_back("copter");
    newUnitData.classifications.push_back("airborne");
    newUnitData.classifications.push_back("non_combat");
    newUnitData.classifications.push_back("transport");
    newUnitData.classifications.push_back("transport_infantry");
  }
  {
    auto& newUnitData = unitData["battle_copter"];
    newUnitData.classifications.push_back("copter");
    newUnitData.classifications.push_back("airborne");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["fighter"];
    newUnitData.classifications.push_back("plane");
    newUnitData.classifications.push_back("airborne");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["bomber"];
    newUnitData.classifications.push_back("plane");
    newUnitData.classifications.push_back("airborne");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["stealth"];
    newUnitData.classifications.push_back("plane");
    newUnitData.classifications.push_back("airborne");
    newUnitData.classifications.push_back("direct");
    newUnitData.classifications.push_back("stealthy");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["black_bomb"];
    newUnitData.classifications.push_back("plane");
    newUnitData.classifications.push_back("airborne");
    newUnitData.classifications.push_back("self_destruct");
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["black_boat"];
    newUnitData.classifications.push_back("naval");
    newUnitData.classifications.push_back("transport");
    newUnitData.classifications.push_back("transport_infantry");
    newUnitData.classifications.push_back("non_combat");
    newUnitData.classifications.push_back("supply");
  }
  {
    auto& newUnitData = unitData["lander"];
    newUnitData.classifications.push_back("naval");
    newUnitData.classifications.push_back("transport");
    newUnitData.classifications.push_back("transport_land");
    newUnitData.classifications.push_back("non_combat");
  }
  {
    auto& newUnitData = unitData["cruiser"];
    newUnitData.classifications.push_back("naval");
    newUnitData.classifications.push_back("transport");
    newUnitData.classifications.push_back("transport_copters");
    newUnitData.classifications.push_back("direct");
    newUnitData.weaponRequiresAmmo.push_back(true);
    newUnitData.weaponRequiresAmmo.push_back(false);
  }
  {
    auto& newUnitData = unitData["battleship"];
    newUnitData.classifications.push_back("naval");
    newUnitData.classifications.push_back("indirect");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["submarine"];
    newUnitData.classifications.push_back("naval");
    newUnitData.classifications.push_back("direct");
    newUnitData.classifications.push_back("stealthy");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  {
    auto& newUnitData = unitData["carrier"];
    newUnitData.classifications.push_back("naval");
    newUnitData.classifications.push_back("indirect");
    newUnitData.weaponRequiresAmmo.push_back(true);
  }
  unitMatchups["anti_air"]["anti_air"].first = 45;
  unitMatchups["anti_air"]["apc"].first = 50;
  unitMatchups["anti_air"]["artillery"].first = 50;
  unitMatchups["anti_air"]["battle_copter"].first = 120;
  unitMatchups["anti_air"]["black_bomb"].first = 120;
  unitMatchups["anti_air"]["bomber"].first = 75;
  unitMatchups["anti_air"]["fighter"].first = 65;
  unitMatchups["anti_air"]["infantry"].first = 105;
  unitMatchups["anti_air"]["mech"].first = 105;
  unitMatchups["anti_air"]["medium_tank"].first = 10;
  unitMatchups["anti_air"]["mega_tank"].first = 1;
  unitMatchups["anti_air"]["missile"].first = 55;
  unitMatchups["anti_air"]["neo_tank"].first = 5;
  unitMatchups["anti_air"]["piperunner"].first = 25;
  unitMatchups["anti_air"]["recon"].first = 60;
  unitMatchups["anti_air"]["rocket"].first = 55;
  unitMatchups["anti_air"]["stealth"].first = 75;
  unitMatchups["anti_air"]["tank"].first = 25;
  unitMatchups["anti_air"]["transport_copter"].first = 120;
  unitMatchups["artillery"]["anti_air"].first = 75;
  unitMatchups["artillery"]["apc"].first = 70;
  unitMatchups["artillery"]["artillery"].first = 75;
  unitMatchups["artillery"]["battleship"].first = 40;
  unitMatchups["artillery"]["black_boat"].first = 55;
  unitMatchups["artillery"]["carrier"].first = 45;
  unitMatchups["artillery"]["cruiser"].first = 65;
  unitMatchups["artillery"]["infantry"].first = 90;
  unitMatchups["artillery"]["lander"].first = 55;
  unitMatchups["artillery"]["mech"].first = 85;
  unitMatchups["artillery"]["medium_tank"].first = 45;
  unitMatchups["artillery"]["mega_tank"].first = 15;
  unitMatchups["artillery"]["missile"].first = 80;
  unitMatchups["artillery"]["neo_tank"].first = 40;
  unitMatchups["artillery"]["piperunner"].first = 70;
  unitMatchups["artillery"]["recon"].first = 80;
  unitMatchups["artillery"]["rocket"].first = 80;
  unitMatchups["artillery"]["submarine"].first = 60;
  unitMatchups["artillery"]["tank"].first = 70;
  unitMatchups["battle_copter"]["anti_air"].first = 25;
  unitMatchups["battle_copter"]["anti_air"].second = 6;
  unitMatchups["battle_copter"]["apc"].first = 60;
  unitMatchups["battle_copter"]["apc"].second = 20;
  unitMatchups["battle_copter"]["artillery"].first = 65;
  unitMatchups["battle_copter"]["artillery"].second = 25;
  unitMatchups["battle_copter"]["battle_copter"].second = 65;
  unitMatchups["battle_copter"]["battleship"].first = 25;
  unitMatchups["battle_copter"]["black_boat"].first = 25;
  unitMatchups["battle_copter"]["carrier"].first = 25;
  unitMatchups["battle_copter"]["cruiser"].first = 55;
  unitMatchups["battle_copter"]["infantry"].second = 75;
  unitMatchups["battle_copter"]["lander"].first = 25;
  unitMatchups["battle_copter"]["mech"].second = 75;
  unitMatchups["battle_copter"]["medium_tank"].first = 25;
  unitMatchups["battle_copter"]["medium_tank"].second = 1;
  unitMatchups["battle_copter"]["mega_tank"].first = 10;
  unitMatchups["battle_copter"]["mega_tank"].second = 1;
  unitMatchups["battle_copter"]["missile"].first = 65;
  unitMatchups["battle_copter"]["missile"].second = 35;
  unitMatchups["battle_copter"]["neo_tank"].first = 20;
  unitMatchups["battle_copter"]["neo_tank"].second = 1;
  unitMatchups["battle_copter"]["piperunner"].first = 55;
  unitMatchups["battle_copter"]["piperunner"].second = 6;
  unitMatchups["battle_copter"]["recon"].first = 55;
  unitMatchups["battle_copter"]["recon"].second = 30;
  unitMatchups["battle_copter"]["rocket"].first = 65;
  unitMatchups["battle_copter"]["rocket"].second = 35;
  unitMatchups["battle_copter"]["submarine"].first = 25;
  unitMatchups["battle_copter"]["tank"].first = 55;
  unitMatchups["battle_copter"]["tank"].second = 6;
  unitMatchups["battle_copter"]["transport_copter"].second = 95;
  unitMatchups["battleship"]["anti_air"].first = 85;
  unitMatchups["battleship"]["apc"].first = 80;
  unitMatchups["battleship"]["artillery"].first = 80;
  unitMatchups["battleship"]["battleship"].first = 50;
  unitMatchups["battleship"]["black_boat"].first = 95;
  unitMatchups["battleship"]["carrier"].first = 60;
  unitMatchups["battleship"]["cruiser"].first = 95;
  unitMatchups["battleship"]["infantry"].first = 95;
  unitMatchups["battleship"]["lander"].first = 95;
  unitMatchups["battleship"]["mech"].first = 90;
  unitMatchups["battleship"]["medium_tank"].first = 55;
  unitMatchups["battleship"]["mega_tank"].first = 25;
  unitMatchups["battleship"]["missile"].first = 90;
  unitMatchups["battleship"]["neo_tank"].first = 50;
  unitMatchups["battleship"]["piperunner"].first = 80;
  unitMatchups["battleship"]["recon"].first = 90;
  unitMatchups["battleship"]["rocket"].first = 85;
  unitMatchups["battleship"]["submarine"].first = 95;
  unitMatchups["battleship"]["tank"].first = 80;
  unitMatchups["bomber"]["anti_air"].first = 95;
  unitMatchups["bomber"]["apc"].first = 105;
  unitMatchups["bomber"]["artillery"].first = 105;
  unitMatchups["bomber"]["battleship"].first = 75;
  unitMatchups["bomber"]["black_boat"].first = 95;
  unitMatchups["bomber"]["carrier"].first = 75;
  unitMatchups["bomber"]["cruiser"].first = 85;
  unitMatchups["bomber"]["infantry"].first = 110;
  unitMatchups["bomber"]["lander"].first = 95;
  unitMatchups["bomber"]["mech"].first = 110;
  unitMatchups["bomber"]["medium_tank"].first = 95;
  unitMatchups["bomber"]["mega_tank"].first = 35;
  unitMatchups["bomber"]["missile"].first = 105;
  unitMatchups["bomber"]["neo_tank"].first = 90;
  unitMatchups["bomber"]["piperunner"].first = 105;
  unitMatchups["bomber"]["recon"].first = 105;
  unitMatchups["bomber"]["rocket"].first = 105;
  unitMatchups["bomber"]["submarine"].first = 95;
  unitMatchups["bomber"]["tank"].first = 105;
  unitMatchups["carrier"]["battle_copter"].first = 115;
  unitMatchups["carrier"]["black_bomb"].first = 120;
  unitMatchups["carrier"]["bomber"].first = 100;
  unitMatchups["carrier"]["fighter"].first = 100;
  unitMatchups["carrier"]["stealth"].first = 100;
  unitMatchups["carrier"]["transport_copter"].first = 115;
  unitMatchups["cruiser"]["battle_copter"].second = 115;
  unitMatchups["cruiser"]["black_boat"].first = 25;
  unitMatchups["cruiser"]["black_bomb"].second = 120;
  unitMatchups["cruiser"]["bomber"].second = 65;
  unitMatchups["cruiser"]["carrier"].first = 5;
  unitMatchups["cruiser"]["fighter"].second = 55;
  unitMatchups["cruiser"]["stealth"].second = 100;
  unitMatchups["cruiser"]["submarine"].first = 90;
  unitMatchups["cruiser"]["transport_copter"].second = 115;
  unitMatchups["fighter"]["battle_copter"].first = 100;
  unitMatchups["fighter"]["black_bomb"].first = 120;
  unitMatchups["fighter"]["bomber"].first = 100;
  unitMatchups["fighter"]["fighter"].first = 55;
  unitMatchups["fighter"]["stealth"].first = 85;
  unitMatchups["fighter"]["transport_copter"].first = 100;
  unitMatchups["infantry"]["anti_air"].first = 5;
  unitMatchups["infantry"]["apc"].first = 14;
  unitMatchups["infantry"]["artillery"].first = 15;
  unitMatchups["infantry"]["battle_copter"].first = 7;
  unitMatchups["infantry"]["infantry"].first = 55;
  unitMatchups["infantry"]["mech"].first = 45;
  unitMatchups["infantry"]["medium_tank"].first = 1;
  unitMatchups["infantry"]["mega_tank"].first = 1;
  unitMatchups["infantry"]["missile"].first = 26;
  unitMatchups["infantry"]["neo_tank"].first = 1;
  unitMatchups["infantry"]["piperunner"].first = 5;
  unitMatchups["infantry"]["recon"].first = 12;
  unitMatchups["infantry"]["rocket"].first = 25;
  unitMatchups["infantry"]["tank"].first = 5;
  unitMatchups["infantry"]["transport_copter"].first = 30;
  unitMatchups["mech"]["anti_air"].first = 65;
  unitMatchups["mech"]["anti_air"].second = 6;
  unitMatchups["mech"]["apc"].first = 75;
  unitMatchups["mech"]["apc"].second = 20;
  unitMatchups["mech"]["artillery"].first = 70;
  unitMatchups["mech"]["artillery"].second = 32;
  unitMatchups["mech"]["battle_copter"].second = 9;
  unitMatchups["mech"]["infantry"].second = 65;
  unitMatchups["mech"]["mech"].second = 55;
  unitMatchups["mech"]["medium_tank"].first = 15;
  unitMatchups["mech"]["medium_tank"].second = 1;
  unitMatchups["mech"]["mega_tank"].first = 5;
  unitMatchups["mech"]["mega_tank"].second = 1;
  unitMatchups["mech"]["missile"].first = 85;
  unitMatchups["mech"]["missile"].second = 35;
  unitMatchups["mech"]["neo_tank"].first = 15;
  unitMatchups["mech"]["neo_tank"].second = 1;
  unitMatchups["mech"]["piperunner"].first = 55;
  unitMatchups["mech"]["piperunner"].second = 6;
  unitMatchups["mech"]["recon"].first = 85;
  unitMatchups["mech"]["recon"].second = 18;
  unitMatchups["mech"]["rocket"].first = 85;
  unitMatchups["mech"]["rocket"].second = 35;
  unitMatchups["mech"]["tank"].first = 55;
  unitMatchups["mech"]["tank"].second = 6;
  unitMatchups["mech"]["transport_copter"].second = 35;
  unitMatchups["medium_tank"]["anti_air"].first = 105;
  unitMatchups["medium_tank"]["anti_air"].second = 7;
  unitMatchups["medium_tank"]["apc"].first = 105;
  unitMatchups["medium_tank"]["apc"].second = 45;
  unitMatchups["medium_tank"]["artillery"].first = 105;
  unitMatchups["medium_tank"]["artillery"].second = 45;
  unitMatchups["medium_tank"]["battle_copter"].second = 12;
  unitMatchups["medium_tank"]["battleship"].first = 10;
  unitMatchups["medium_tank"]["black_boat"].first = 35;
  unitMatchups["medium_tank"]["carrier"].first = 10;
  unitMatchups["medium_tank"]["cruiser"].first = 45;
  unitMatchups["medium_tank"]["infantry"].second = 105;
  unitMatchups["medium_tank"]["lander"].first = 35;
  unitMatchups["medium_tank"]["mech"].second = 95;
  unitMatchups["medium_tank"]["medium_tank"].first = 55;
  unitMatchups["medium_tank"]["medium_tank"].second = 1;
  unitMatchups["medium_tank"]["mega_tank"].first = 25;
  unitMatchups["medium_tank"]["mega_tank"].second = 1;
  unitMatchups["medium_tank"]["missile"].first = 105;
  unitMatchups["medium_tank"]["missile"].second = 35;
  unitMatchups["medium_tank"]["neo_tank"].first = 45;
  unitMatchups["medium_tank"]["neo_tank"].second = 1;
  unitMatchups["medium_tank"]["piperunner"].first = 85;
  unitMatchups["medium_tank"]["piperunner"].second = 8;
  unitMatchups["medium_tank"]["recon"].first = 105;
  unitMatchups["medium_tank"]["recon"].second = 45;
  unitMatchups["medium_tank"]["rocket"].first = 105;
  unitMatchups["medium_tank"]["rocket"].second = 55;
  unitMatchups["medium_tank"]["submarine"].first = 10;
  unitMatchups["medium_tank"]["tank"].first = 85;
  unitMatchups["medium_tank"]["tank"].second = 8;
  unitMatchups["medium_tank"]["transport_copter"].second = 45;
  unitMatchups["mega_tank"]["anti_air"].first = 195;
  unitMatchups["mega_tank"]["anti_air"].second = 17;
  unitMatchups["mega_tank"]["apc"].first = 195;
  unitMatchups["mega_tank"]["apc"].second = 65;
  unitMatchups["mega_tank"]["artillery"].first = 195;
  unitMatchups["mega_tank"]["artillery"].second = 65;
  unitMatchups["mega_tank"]["battle_copter"].second = 22;
  unitMatchups["mega_tank"]["battleship"].first = 45;
  unitMatchups["mega_tank"]["black_boat"].first = 105;
  unitMatchups["mega_tank"]["carrier"].first = 45;
  unitMatchups["mega_tank"]["cruiser"].first = 65;
  unitMatchups["mega_tank"]["infantry"].second = 135;
  unitMatchups["mega_tank"]["lander"].first = 75;
  unitMatchups["mega_tank"]["mech"].second = 125;
  unitMatchups["mega_tank"]["medium_tank"].first = 125;
  unitMatchups["mega_tank"]["medium_tank"].second = 1;
  unitMatchups["mega_tank"]["mega_tank"].first = 65;
  unitMatchups["mega_tank"]["mega_tank"].second = 1;
  unitMatchups["mega_tank"]["missile"].first = 195;
  unitMatchups["mega_tank"]["missile"].second = 55;
  unitMatchups["mega_tank"]["neo_tank"].first = 115;
  unitMatchups["mega_tank"]["neo_tank"].second = 1;
  unitMatchups["mega_tank"]["piperunner"].first = 180;
  unitMatchups["mega_tank"]["piperunner"].second = 10;
  unitMatchups["mega_tank"]["recon"].first = 195;
  unitMatchups["mega_tank"]["recon"].second = 65;
  unitMatchups["mega_tank"]["rocket"].first = 195;
  unitMatchups["mega_tank"]["rocket"].second = 75;
  unitMatchups["mega_tank"]["submarine"].first = 40;
  unitMatchups["mega_tank"]["tank"].first = 180;
  unitMatchups["mega_tank"]["tank"].second = 10;
  unitMatchups["mega_tank"]["transport_copter"].second = 55;
  unitMatchups["missile"]["battle_copter"].first = 120;
  unitMatchups["missile"]["black_bomb"].first = 120;
  unitMatchups["missile"]["bomber"].first = 100;
  unitMatchups["missile"]["fighter"].first = 100;
  unitMatchups["missile"]["stealth"].first = 100;
  unitMatchups["missile"]["transport_copter"].first = 120;
  unitMatchups["neo_tank"]["anti_air"].first = 115;
  unitMatchups["neo_tank"]["anti_air"].second = 17;
  unitMatchups["neo_tank"]["apc"].first = 125;
  unitMatchups["neo_tank"]["apc"].second = 65;
  unitMatchups["neo_tank"]["artillery"].first = 115;
  unitMatchups["neo_tank"]["artillery"].second = 65;
  unitMatchups["neo_tank"]["battle_copter"].second = 22;
  unitMatchups["neo_tank"]["battleship"].first = 15;
  unitMatchups["neo_tank"]["black_boat"].first = 40;
  unitMatchups["neo_tank"]["carrier"].first = 15;
  unitMatchups["neo_tank"]["cruiser"].first = 50;
  unitMatchups["neo_tank"]["infantry"].second = 125;
  unitMatchups["neo_tank"]["lander"].first = 50;
  unitMatchups["neo_tank"]["mech"].second = 115;
  unitMatchups["neo_tank"]["medium_tank"].first = 75;
  unitMatchups["neo_tank"]["medium_tank"].second = 1;
  unitMatchups["neo_tank"]["mega_tank"].first = 35;
  unitMatchups["neo_tank"]["mega_tank"].second = 1;
  unitMatchups["neo_tank"]["missile"].first = 125;
  unitMatchups["neo_tank"]["missile"].second = 55;
  unitMatchups["neo_tank"]["neo_tank"].first = 55;
  unitMatchups["neo_tank"]["neo_tank"].second = 1;
  unitMatchups["neo_tank"]["piperunner"].first = 105;
  unitMatchups["neo_tank"]["piperunner"].second = 10;
  unitMatchups["neo_tank"]["recon"].first = 125;
  unitMatchups["neo_tank"]["recon"].second = 65;
  unitMatchups["neo_tank"]["rocket"].first = 125;
  unitMatchups["neo_tank"]["rocket"].second = 75;
  unitMatchups["neo_tank"]["submarine"].first = 15;
  unitMatchups["neo_tank"]["tank"].first = 105;
  unitMatchups["neo_tank"]["tank"].second = 10;
  unitMatchups["neo_tank"]["transport_copter"].second = 55;
  unitMatchups["piperunner"]["anti_air"].first = 85;
  unitMatchups["piperunner"]["apc"].first = 80;
  unitMatchups["piperunner"]["artillery"].first = 80;
  unitMatchups["piperunner"]["battle_copter"].first = 105;
  unitMatchups["piperunner"]["battleship"].first = 55;
  unitMatchups["piperunner"]["black_boat"].first = 60;
  unitMatchups["piperunner"]["black_bomb"].first = 120;
  unitMatchups["piperunner"]["bomber"].first = 75;
  unitMatchups["piperunner"]["carrier"].first = 60;
  unitMatchups["piperunner"]["cruiser"].first = 85;
  unitMatchups["piperunner"]["fighter"].first = 65;
  unitMatchups["piperunner"]["infantry"].first = 95;
  unitMatchups["piperunner"]["lander"].first = 60;
  unitMatchups["piperunner"]["mech"].first = 90;
  unitMatchups["piperunner"]["medium_tank"].first = 55;
  unitMatchups["piperunner"]["mega_tank"].first = 25;
  unitMatchups["piperunner"]["missile"].first = 90;
  unitMatchups["piperunner"]["neo_tank"].first = 50;
  unitMatchups["piperunner"]["piperunner"].first = 80;
  unitMatchups["piperunner"]["recon"].first = 90;
  unitMatchups["piperunner"]["rocket"].first = 85;
  unitMatchups["piperunner"]["stealth"].first = 75;
  unitMatchups["piperunner"]["submarine"].first = 85;
  unitMatchups["piperunner"]["tank"].first = 80;
  unitMatchups["piperunner"]["transport_copter"].first = 105;
  unitMatchups["recon"]["anti_air"].first = 4;
  unitMatchups["recon"]["apc"].first = 45;
  unitMatchups["recon"]["artillery"].first = 45;
  unitMatchups["recon"]["battle_copter"].first = 12;
  unitMatchups["recon"]["infantry"].first = 70;
  unitMatchups["recon"]["mech"].first = 65;
  unitMatchups["recon"]["medium_tank"].first = 1;
  unitMatchups["recon"]["mega_tank"].first = 1;
  unitMatchups["recon"]["missile"].first = 28;
  unitMatchups["recon"]["neo_tank"].first = 1;
  unitMatchups["recon"]["piperunner"].first = 6;
  unitMatchups["recon"]["recon"].first = 35;
  unitMatchups["recon"]["rocket"].first = 55;
  unitMatchups["recon"]["tank"].first = 6;
  unitMatchups["recon"]["transport_copter"].first = 35;
  unitMatchups["rocket"]["anti_air"].first = 85;
  unitMatchups["rocket"]["apc"].first = 80;
  unitMatchups["rocket"]["artillery"].first = 80;
  unitMatchups["rocket"]["battleship"].first = 55;
  unitMatchups["rocket"]["black_boat"].first = 60;
  unitMatchups["rocket"]["carrier"].first = 60;
  unitMatchups["rocket"]["cruiser"].first = 85;
  unitMatchups["rocket"]["infantry"].first = 95;
  unitMatchups["rocket"]["lander"].first = 60;
  unitMatchups["rocket"]["mech"].first = 90;
  unitMatchups["rocket"]["medium_tank"].first = 55;
  unitMatchups["rocket"]["mega_tank"].first = 25;
  unitMatchups["rocket"]["missile"].first = 90;
  unitMatchups["rocket"]["neo_tank"].first = 50;
  unitMatchups["rocket"]["piperunner"].first = 80;
  unitMatchups["rocket"]["recon"].first = 90;
  unitMatchups["rocket"]["rocket"].first = 85;
  unitMatchups["rocket"]["submarine"].first = 85;
  unitMatchups["rocket"]["tank"].first = 80;
  unitMatchups["stealth"]["anti_air"].first = 50;
  unitMatchups["stealth"]["apc"].first = 85;
  unitMatchups["stealth"]["artillery"].first = 75;
  unitMatchups["stealth"]["battle_copter"].first = 85;
  unitMatchups["stealth"]["battleship"].first = 45;
  unitMatchups["stealth"]["black_boat"].first = 65;
  unitMatchups["stealth"]["black_bomb"].first = 120;
  unitMatchups["stealth"]["bomber"].first = 70;
  unitMatchups["stealth"]["carrier"].first = 45;
  unitMatchups["stealth"]["cruiser"].first = 35;
  unitMatchups["stealth"]["fighter"].first = 45;
  unitMatchups["stealth"]["infantry"].first = 90;
  unitMatchups["stealth"]["lander"].first = 65;
  unitMatchups["stealth"]["mech"].first = 90;
  unitMatchups["stealth"]["medium_tank"].first = 70;
  unitMatchups["stealth"]["mega_tank"].first = 15;
  unitMatchups["stealth"]["missile"].first = 85;
  unitMatchups["stealth"]["neo_tank"].first = 60;
  unitMatchups["stealth"]["piperunner"].first = 80;
  unitMatchups["stealth"]["recon"].first = 85;
  unitMatchups["stealth"]["rocket"].first = 85;
  unitMatchups["stealth"]["stealth"].first = 55;
  unitMatchups["stealth"]["submarine"].first = 55;
  unitMatchups["stealth"]["tank"].first = 75;
  unitMatchups["stealth"]["transport_copter"].first = 95;
  unitMatchups["submarine"]["battleship"].first = 55;
  unitMatchups["submarine"]["black_boat"].first = 95;
  unitMatchups["submarine"]["carrier"].first = 75;
  unitMatchups["submarine"]["cruiser"].first = 25;
  unitMatchups["submarine"]["lander"].first = 95;
  unitMatchups["submarine"]["submarine"].first = 55;
  unitMatchups["tank"]["anti_air"].first = 65;
  unitMatchups["tank"]["anti_air"].second = 6;
  unitMatchups["tank"]["apc"].first = 75;
  unitMatchups["tank"]["apc"].second = 45;
  unitMatchups["tank"]["artillery"].first = 70;
  unitMatchups["tank"]["artillery"].second = 45;
  unitMatchups["tank"]["battle_copter"].second = 10;
  unitMatchups["tank"]["battleship"].first = 1;
  unitMatchups["tank"]["black_boat"].first = 10;
  unitMatchups["tank"]["carrier"].first = 1;
  unitMatchups["tank"]["cruiser"].first = 5;
  unitMatchups["tank"]["infantry"].second = 75;
  unitMatchups["tank"]["lander"].first = 10;
  unitMatchups["tank"]["mech"].second = 70;
  unitMatchups["tank"]["medium_tank"].first = 15;
  unitMatchups["tank"]["medium_tank"].second = 1;
  unitMatchups["tank"]["mega_tank"].first = 10;
  unitMatchups["tank"]["mega_tank"].second = 1;
  unitMatchups["tank"]["missile"].first = 85;
  unitMatchups["tank"]["missile"].second = 30;
  unitMatchups["tank"]["neo_tank"].first = 15;
  unitMatchups["tank"]["neo_tank"].second = 1;
  unitMatchups["tank"]["piperunner"].first = 55;
  unitMatchups["tank"]["piperunner"].second = 6;
  unitMatchups["tank"]["recon"].first = 85;
  unitMatchups["tank"]["recon"].second = 40;
  unitMatchups["tank"]["rocket"].first = 85;
  unitMatchups["tank"]["rocket"].second = 55;
  unitMatchups["tank"]["submarine"].first = 1;
  unitMatchups["tank"]["tank"].first = 55;
  unitMatchups["tank"]["tank"].second = 6;
  unitMatchups["tank"]["transport_copter"].second = 40;
  struct Classification {
    std::string name;
    bool negate = false;
  };
  struct PassiveEffect {
    int firepower = 0, defense = 0, terrainStars = 0, goodLuck = 0, badLuck = 0, counterFireBonus = 0, propertyFirepower = 0, towerFirepower = 0,
        towerDefense = 0, indirectDefense = 0, enemyTerrainStars = 0;
    bool starsFirepower = false;
    std::vector<Classification> requiredClassifications;
    std::vector<std::string> validTerrains;
  };
  struct CommanderData {
    std::vector<PassiveEffect> d2d, cop, scop;
  };
  CommanderData baseline;
  std::map<std::string, CommanderData> commanderPowers;

  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 9;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.towerFirepower = 10;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 100;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("airborne", true);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("footsoldier", false);
      newEffect.validTerrains.emplace_back("mountain");
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    baseline = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["adder"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["andy"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = -10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["colin"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("naval", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = -30;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("airborne", false);
      newData.d2d.push_back(newEffect);
    }
    commanderPowers["drake"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 15;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("airborne", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = -30;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("naval", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 5;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("airborne", false);
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 5;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("airborne", false);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["eagle"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 15;
      newEffect.badLuck = 9;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 25;
      newEffect.badLuck = 10;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 65;
      newEffect.badLuck = 30;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["flak"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 30;
      newEffect.defense = -20;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 50;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["grimm"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("indirect", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = -20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("direct", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("indirect", false);
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("indirect", false);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["grit"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["hachi"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    commanderPowers["hawke"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("plains");
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("plains");
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("vehicle", false);
      newEffect.requiredClassifications.emplace_back("indirect", false);
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 30;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("plains");
      newData.scop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("vehicle", false);
      newEffect.requiredClassifications.emplace_back("indirect", false);
      newData.scop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("vehicle", false);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["jake"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.towerDefense = 10;
      newEffect.indirectDefense = 20;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.towerFirepower = 10;
      newEffect.towerDefense = 10;
      newEffect.indirectDefense = 20;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.towerFirepower = 20;
      newEffect.towerDefense = 20;
      newEffect.indirectDefense = 60;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["javier"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("vehicle", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = -10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("vehicle", true);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("vehicle", false);
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 30;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("vehicle", false);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["jess"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 20;
      newEffect.badLuck = 14;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 25;
      newEffect.badLuck = 10;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 65;
      newEffect.badLuck = 30;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["jugger"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 30;
      newEffect.defense = 30;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 20;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 50;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["kanbei"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 40;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("city");
      newEffect.validTerrains.emplace_back("base");
      newEffect.validTerrains.emplace_back("airport");
      newEffect.validTerrains.emplace_back("port");
      newEffect.validTerrains.emplace_back("headquarters");
      newEffect.validTerrains.emplace_back("tower");
      newEffect.validTerrains.emplace_back("lab");
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 40;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("city");
      newEffect.validTerrains.emplace_back("base");
      newEffect.validTerrains.emplace_back("airport");
      newEffect.validTerrains.emplace_back("port");
      newEffect.validTerrains.emplace_back("headquarters");
      newEffect.validTerrains.emplace_back("tower");
      newEffect.validTerrains.emplace_back("lab");
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 90;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("city");
      newEffect.validTerrains.emplace_back("base");
      newEffect.validTerrains.emplace_back("airport");
      newEffect.validTerrains.emplace_back("port");
      newEffect.validTerrains.emplace_back("headquarters");
      newEffect.validTerrains.emplace_back("tower");
      newEffect.validTerrains.emplace_back("lab");
      newEffect.propertyFirepower = 3;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["kindle"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("road");
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("road");
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.validTerrains.emplace_back("road");
      newData.scop.push_back(newEffect);
    }
    commanderPowers["koal"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 1;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 100;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("airborne", true);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["lash"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("direct", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = -10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("indirect", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("direct", false);
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 30;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("direct", false);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["max"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 10;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 40;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 80;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["nell"] = newData;
  }
  {
    CommanderData newData;
    commanderPowers["olaf"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 30;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    commanderPowers["rachel"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 30;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("footsoldier", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = -10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("direct", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("transport", false);
      newEffect.requiredClassifications.emplace_back("non_combat", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("footsoldier", false);
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 40;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("footsoldier", false);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["sami"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["sasha"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 40;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("footsoldier", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 50;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("copter", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = -10;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("copter", true);
      newEffect.requiredClassifications.emplace_back("footsoldier", true);
      newEffect.requiredClassifications.emplace_back("airborne", true);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("transport", false);
      newEffect.requiredClassifications.emplace_back("non_combat", false);
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 15;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("copter", false);
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 15;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.requiredClassifications.emplace_back("copter", false);
      newData.scop.push_back(newEffect);
    }
    commanderPowers["sensei"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 9;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 50;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["sonja"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 4;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.enemyTerrainStars = -1;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.enemyTerrainStars = -1;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 0;
      newEffect.defense = 0;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newEffect.enemyTerrainStars = -2;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["sonjads"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 20;
      newEffect.defense = 20;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.cop.push_back(newEffect);
    }
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.scop.push_back(newEffect);
    }
    commanderPowers["sturm"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = -20;
      newEffect.defense = 20;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    commanderPowers["awbwsturm"] = newData;
  }
  {
    CommanderData newData;
    {
      PassiveEffect newEffect;
      newEffect.firepower = 10;
      newEffect.defense = 10;
      newEffect.goodLuck = 0;
      newEffect.badLuck = 0;
      newEffect.terrainStars = 0;
      newEffect.starsFirepower = 0;
      newEffect.counterFireBonus = 0;
      newData.d2d.push_back(newEffect);
    }
    commanderPowers["von_bolt"] = newData;
  }

  std::pair<UnitProperties, UnitProperties> ret;
  try {
    auto& [unitPropertiesA, unitPropertiesB] = ret;
    unitPropertiesA.baseDamage = {};
    unitPropertiesA.goodLuck = 0;
    unitPropertiesB.baseDamage = {};
    unitPropertiesB.goodLuck = 0;
    auto check = [](std::string const& name1, std::string const& name2, auto&& data, std::string_view type) {
      if (data.find(name1) == data.end() || data.find(name2) == data.end()) {
        std::stringstream ss;
        std::print(ss, "Unable to find {} Names: '{}', '{}'", type, name1, name2);
        throw std::runtime_error(ss.str());
      }
    };
    check(attackingUnit.name, defendingUnit.name, unitData, "Unit");
    auto const& unitDataA = unitData.at(attackingUnit.name);
    auto const& unitDataB = unitData.at(defendingUnit.name);
    check(attackingUnit.commander, defendingUnit.commander, commanderPowers, "Commander");
    auto const& commanderDataA = commanderPowers.at(attackingUnit.commander);
    auto const& commanderDataB = commanderPowers.at(defendingUnit.commander);
    check(attackingUnit.terrain, defendingUnit.terrain, terrainStars, "Terrain");
    auto const& starsA = terrainStars.at(attackingUnit.terrain);
    auto const& starsB = terrainStars.at(defendingUnit.terrain);

    auto getMatchupPointer = [&](std::string const& attackerName,
                                 std::string const& defenderName) -> std::pair<std::optional<int>, std::optional<int>> const* {
      if (auto it1 = unitMatchups.find(attackerName); it1 != unitMatchups.end()) {
        if (auto it2 = it1->second.find(defenderName); it2 != it1->second.end()) {
          return &it2->second;
        }
      }
      return nullptr;
    };
    auto setBaseDamage = [&](std::pair<std::optional<int>, std::optional<int>> const* ptr, UnitData const& data, UnitType const& type,
                             bool attacking = true) -> std::optional<int> {
      if (!ptr) {
        return {};
      }
      if (std::find(data.classifications.begin(), data.classifications.end(), "indirect") != data.classifications.end() && !attacking) {
        return {};
      }
      if (ptr->first && ((data.weaponRequiresAmmo.at(0) && type.hasAmmo) || !data.weaponRequiresAmmo.at(0))) {
        return ptr->first;
      }
      if (ptr->second && ((data.weaponRequiresAmmo.at(1) && type.hasAmmo) || !data.weaponRequiresAmmo.at(1))) {
        return ptr->second;
      }
      return {};
    };
    // Set the base damage
    auto attackerMatchup = getMatchupPointer(attackingUnit.name, defendingUnit.name);
    unitPropertiesA.baseDamage = setBaseDamage(attackerMatchup, unitDataA, attackingUnit, true);
    auto defenderMatchup = getMatchupPointer(defendingUnit.name, attackingUnit.name);
    unitPropertiesB.baseDamage = setBaseDamage(defenderMatchup, unitDataB, defendingUnit, false);

    bool attackerIndirect = std::ranges::find(unitDataA.classifications, "indirect") != unitDataA.classifications.end();
    if (attackerIndirect) {
      unitPropertiesB.baseDamage = {};
    }

    auto addCOEffects = [&](UnitProperties& properties, UnitData const& unitData, CommanderData const& commanderData, std::string const& terrain,
                            Integer terrainStars, Integer urban, int copState, bool attacking = true) {
      auto addAllPassiveEffects = [&](std::vector<PassiveEffect> const& effects) {
        for (PassiveEffect const& effect : effects) {
          auto const& requiredClassifications = effect.requiredClassifications;
          auto const& validTerrain = effect.validTerrains;
          if (validTerrain.size() > 0 && std::find(validTerrain.begin(), validTerrain.end(), terrain) == validTerrain.end()) {
            continue;
          }
          if (requiredClassifications.size() > 0) {
            bool classificationsMatch = true;
            for (auto const& classification : requiredClassifications) {
              if (auto it = std::find(unitData.classifications.begin(), unitData.classifications.end(), classification.name);
                  (classification.negate && it != unitData.classifications.end()) ||
                  (!classification.negate && it == unitData.classifications.end())) {
                classificationsMatch = false;
                break;
              }
            }
            if (!classificationsMatch) {
              continue;
            }
          }
          properties.firepowerBoost += effect.firepower;
          properties.defenseBoost += effect.defense;
          properties.goodLuck += effect.goodLuck;
          properties.badLuck += effect.badLuck;
          properties.firepowerBoost += effect.propertyFirepower * urban;
          properties.towerFirepower += effect.towerFirepower;
          properties.towerDefense += effect.towerDefense;
          properties.indirectDefense += effect.indirectDefense;
          if (!attacking) {
            properties.firepowerBoost += effect.counterFireBonus;
          }
          if (effect.starsFirepower) {
            properties.damageStars = true;
          }
          properties.terrainStars += effect.terrainStars * terrainStars / 100;
          properties.enemyTerrainStars += effect.enemyTerrainStars;
        }
      };

      if (copState >= 0) {
        addAllPassiveEffects(commanderData.d2d);
      }
      if (copState == 1) {
        addAllPassiveEffects(commanderData.cop);
      }
      if (copState == 2) {
        addAllPassiveEffects(commanderData.scop);
      }
    };

    addCOEffects(unitPropertiesA, unitDataA, baseline, attackingUnit.terrain, starsA, attackingUnit.properties, attackingUnit.copState, true);
    addCOEffects(unitPropertiesB, unitDataB, baseline, defendingUnit.terrain, starsB, defendingUnit.properties, defendingUnit.copState, false);

    addCOEffects(unitPropertiesA, unitDataA, commanderDataA, attackingUnit.terrain, starsA, attackingUnit.properties, attackingUnit.copState, true);
    addCOEffects(unitPropertiesB, unitDataB, commanderDataB, defendingUnit.terrain, starsB, defendingUnit.properties, defendingUnit.copState, false);
    unitPropertiesA = unitPropertiesA.addTowers(attackingUnit.towers);
    unitPropertiesB = unitPropertiesB.addTowers(defendingUnit.towers);

    if (attackerIndirect) {
      unitPropertiesB.defenseBoost += unitPropertiesB.indirectDefense;
    }

    return ret;
  } catch (std::runtime_error const& e) {
    return std::unexpected(std::string{e.what()});
  }
}