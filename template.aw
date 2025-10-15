# UNIT DEFINITION
# Name:UnitType|Terrain|CommandingOfficer|COPState|Ammo|CommTowers|Properties|StartingHP|ExtraFirepower|ExtraDefense
#   * Name: unique identifier of the unit. If reused, a new unit will replace the old
#   * UnitType: An AWBW Unit Type. Possible Units:
#     - infantry, mech, recon, tank, apc, artillery, rockets, missile, medium_tank, neo_tank, mega_tank, anti_air, piperunner
#     - transport_copter, battle_copter, bomber, fighter, stealth
#     - black_boat, lander, cruiser, submarine, battleship, carrier
#   * Terrain: Terrain the unit is standing on. Possible Terrains:
#     - road, plains, forest, mountain, silo, city, base, airport, port, headquarters, shoals
#     - river, reef, sea, bridge, pipeline, lab, tower
#   * CommandingOfficer: An AWBW Commanding Officer. Possible COs:
#     - andy, colin, drake, eagle, flak, grimm, grit, hachi, hawke, jake, javier, jess, jugger
#     - kanbei, kindle, koal, lash, max, nell, olaf, rachel, sami, sasha, sensei, sonja
#     - sturm, awbwsturm, von_bolt
#   * COPState: 0 for D2D only, 1 for COP, 2 for SCOP
#   * Ammo: 'true' or empty if the unit has ammo, 'false' if the unit doesn't have ammo.
#   * Towers: Number of Comm Towers controlled by player. Defaults to 0.
#   * Properties: Number of Urban Properties owned by player (only affects Kindle SCOP)
#   * StartingHP: Sets the unit's Hit Points from 0 to 100. Defaults to 100.
#     - May be a comma-delimited list; repeated values will be proportionally weighted higher
#   * ExtraFirepower: Adds a custom amount of firepower to a unit for debugging purposes. Defaults to 0. Accepts integers only, measured in percentage points.
#   * ExtraDefense: Adds a custom amount of defense to a unit for debugging purposes. Defaults to 0. Accepts integers only, measured in percentage points.
# 
# BATTLE DEFINITION
# Attacker>Defender>Filters...
#   * Attacker: refers to the unit identifier of the attacking unit.
#   * Defender: refers to the unit identifier of the defending unit. If this unit belongs to a SCOP'd Sonja, the attacking order will NOT be flipped.
#     - Filters take the form of Type<Value<Option.
#       - Type is one of attackerhp or defenderhp
#       - Value is a comma-delimited list of values to filter the hp to
#       - Option is one of 'equals', 'notequals', 'lessthan', 'greaterthan', 'atleast', 'atmost', 'between'
#       - if Option is 'between' the Values must be exactly two values
#       - Multiple Filters may be applied simultaneously by chaining with '>'
# 
# COMMAND DEFINITION
# Command!Args
#   * print: Args is interpreted as a unit identifier and its current stats are printed out.
#   * echo: Args is printed to the console as-is
# 
# MODIFICATION DEFINITION
# UnitId<Type<Arg<Options
#   * UnitId: Unit Id of the unit that will be modified (with other attributes left alone)
#   * Type: The modification being made. The following are valid:
#     - terrain: the terrain type of the unit, as though it moved
#     - copstate: The CO Power state of the player
#     - ammo: Whether the unit has ammo or not
#     - towers: The number of comm towers the player has
#     - properties: The number of urban properties the player owns
#     - damage: The number of hitpoints to subtract from the unit
#       - Option: NoKill will set the minimum HP after a damage effect to 1.
#     - heal: The number of hitpoints to restore to the unit
#     - roundup: Any fractional HP is rounded up to the nearest whole number (so 1-9 rounds up to 10, 11-19 rounds up to 20, etc.)
#     - filterhp: Filters a unit's HP to only the values that match the predicate.
#       - Option: one of 'equals', 'notequals', 'lessthan', 'greaterthan', 'atleast', 'atmost', 'between'
#       - When option 'equals' or 'notequals' are used, hp values may be comma-delimited.
#       - When option 'between' used, hp values must be exactly two, comma-delimited.

# Context: Sonja is trying to capture a neutral base, and Adder doesn't want to let her. He has two infantry in position, and tries to interrupt
SonjaCaptInf:infantry|base|sonja|||||||
AdderIntInf1:infantry|shoals|adder|||||||

# This move already happened. In-game, we observed that the attacker and defender both ended up at 6 HP Units
# So now we filter the results to determine what the actual distribution of hp is
AdderIntInf1>SonjaCaptInf>defenderhp<51,60<between>attackerhp<51,60<between

echo!### In-Game Results ###
print!AdderIntInf1
print!SonjaCaptInf

# Now, we have a second infantry show up to try to finish the job.
AdderIntInf2:infantry|shoals|adder|||||||

# In-game, the attacking infantry didn't (appear) to take any damage, and the defending infantry was reduced to 1HP
AdderIntInf2>SonjaCaptInf>attackerhp<91<atleast>defenderhp<10<atmost

print!AdderIntInf2
print!SonjaCaptInf

# Now, we roll it all back, and just find out all the possible ranges of outcomes.
SonjaCaptInf:infantry|base|sonja|||||||
AdderIntInf1:infantry|shoals|adder|||||||
AdderIntInf2:infantry|shoals|adder|||||||
AdderIntInf1>SonjaCaptInf
AdderIntInf2>SonjaCaptInf

echo!
echo!
echo!### Theoretical Results ###
print!SonjaCaptInf
print!AdderIntInf1
print!AdderIntInf2