#UNIT DEFINITION
#Name:UnitType|Terrain|CommandingOfficer|COPState|Ammo|CommTowers|Properties|StartingHP|ExtraFirepower|ExtraDefense
#  * Name: unique identifier of the unit. If reused, a new unit will replace the old
#  * UnitType: An AWBW Unit Type. Possible Units:
#    - infantry, mech, recon, tank, apc, artillery, rockets, missile, medium_tank, neo_tank, mega_tank, anti_air, piperunner
#    - transport_copter, battle_copter, bomber, fighter, stealth
#    - black_boat, lander, cruiser, submarine, battleship, carrier
#  * Terrain: Terrain the unit is standing on. Possible Terrains:
#    - road, plains, forest, mountain, silo, city, base, airport, port, headquarters, shoals
#    - river, reef, sea, bridge, pipeline, lab, tower
#  * CommandingOfficer: An AWBW Commanding Officer. Possible COs:
#    - andy, colin, drake, eagle, flak, grimm, grit, hachi, hawke, jake, javier, jess, jugger
#    - kanbei, kindle, koal, lash, max, nell, olaf, rachel, sami, sasha, sensei, sonja
#    - sturm, awbwsturm, von_bolt
#  * COPState: 0 for D2D only, 1 for COP, 2 for SCOP
#  * Ammo: 'true' or empty if the unit has ammo, 'false' if the unit doesn't have ammo.
#  * Towers: Number of Comm Towers controlled by player. Defaults to 0.
#  * Properties: Number of Urban Properties owned by player (only affects Kindle SCOP)
#  * StartingHP: Sets the unit's Hit Points from 0 to 100. Defaults to 100.
#    - May be a comma-delimited list; repeated values will be proportionally weighted higher
#  * ExtraFirepower: Adds a custom amount of firepower to a unit for debugging purposes. Defaults to 0. Accepts integers only, measured in percentage points.
#  * ExtraDefense: Adds a custom amount of defense to a unit for debugging purposes. Defaults to 0. Accepts integers only, measured in percentage points.
#
#BATTLE DEFINITION
#Attacker>Defender
#  * Attacker: refers to the unit identifier of the attacking unit.
#  * Defender: refers to the unit identifier of the defending unit. If this unit belongs to a SCOP'd Sonja, the attacking order will NOT be flipped.
#
#COMMAND DEFINITION
#Command!Args
#  * print: Args is interpreted as a unit identifier and its current stats are printed out.
#  * echo: Args is printed to the console as-is
#
#MODIFICATION DEFINITION
#UnitId<Type<Arg
#  * UnitId: Unit Id of the unit that will be modified (with other attributes left alone)
#  * Type: The modification being made. The following are valid:
#    - terrain: the terrain type of the unit, as though it moved
#    - copstate: The CO Power state of the player
#    - ammo: Whether the unit has ammo or not
#    - towers: The number of comm towers the player has
#    - properties: The number of urban properties the player owns
#    - damage: The number of hitpoints to subtract from the unit
#    - heal: The number of hitpoints to restore to the unit
#    - roundup: Any fractional HP is rounded up to the nearest whole number (so 1-9 rounds up to 10, 11-19 rounds up to 20, etc.)

GrimmTank:tank|plains|grimm|2||1||||
Victim:tank|plains|andy|||||||
GrimmTank<terrain<city
GrimmTank>Victim
print!Victim
print!GrimmTank