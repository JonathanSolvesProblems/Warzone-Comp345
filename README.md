# Concordia University  Department of Computer Science and Software Engineering  Advanced Program Design with C++  COMP 345 --- Fall 2020

# Requirements

- g++: MinGw on Windows (http://www.codebind.com/cprogramming/install-mingw-windows-10-gcc/)

# Building and Running
Each driver (and corresponding `main` function) has a .sh (or .bat for Windows) script for building the executable. The build
scripts were written manually using g++.

Execute the build script for the desired driver, then run the resulting executable. On linux, you must first run
`chmod +x NAME_OF_DRIVER`

# Project Description

All assignments (3) will be about the development of different parts of a project. Thus, every assignment
will build upon code developed in previous assignments. You are expected to work in teams of 5 for every
assignment.

## Problem Statement

The specific project for this semester consists of building a simple "Risk" computer game. The developed
program will have to be compatible with the rules of the "Warzone" version of Risk, which can be accessed
at https://www.warzone.com/. A Warzone game consists of a connected graph map representing a world map,
where each node is a territory they own, from which they can attack adjacent territories to conquer them.
The objective of the game is to conquer all territories on the map.

## Parts/Features of the Game

**Map:** The game map is a connected graph where each node represents a territory owned by one of the
players. Edges between the nodes represent adjacency between territories. The map is divided into subgraphs
that represent continents. A continent is a connected subgraph of the map graph, and every territory belongs
to one and only one continent. Each continent is given a control value (a "bonus" in Warzone terminology)
that determines the number of armies per turn that is given to a player that controls all of it. During
gameplay, every territory belongs to one and only one player and contains one or more armies that belong
to the player owning the territory. In your implementation, it will be expected that the game can be played
on any connected graph that defined by the user before play, saved as a text file representation, and loaded
by the game during play.

**Game:** The game starts by the *startup phase*, where the number of players is determined, then all the
territories are randomly assigned to the players. Then the turn-based main play phase begins, in which all
players are creating a set of battle orders. After all the orders have been given for all players for a turn,
they are executed by the game engine: The orders are:

1. Deployment orders
2. Advance orders
3. Special orders resulting from using cards:
	- **Deploy**: Place some armies on one of the current player's territories.
	- **Advance**: Move some armies from one of the current player's territories (source) to an adjacent
		territory. If the target territory belongs to another player, then the armies are moved to the
		target territory. If the target territory belongs to another player, an attack happens between the
		two territories.
	- **Bomb**: Destroy half of the armies located on an opponent's territory that is adjacent to one of
		the current player's territories.
	- **Blockade**: Triple the number of armies on one of the current player's territories and make it a
		neutral territory.
	- **Airlift**: Advance some armies from one of the current player's territories to any other territory.
	- **Negociate**: Prevent attacks between the current player and another player until the end of the turn.

Each player, in round-robin turn order, give one of their orders. Once all the players have signified that
they don't have any more orders for this turn, the game engine executes all the orders. At the beginning of
every turn, the players are given a number of armies that depends on the number of territories they own (# of
territories owned divided by 3, rounded down). If the player owns all the territories of an entire continent,
then the player is given a number of armies corresponding to the continent's control bonus value. In any case,
the minimal number of reinforcement armies for any player is 3. Once the total number of reinforcements is
determined for the player's turn, the player may start giving deployment orders, i.e. placing their reinforcement
armies on some territories they own. The players may also give advance orders. To do so, the player may choose
one of the territories they own that contains one or more armies and order them to advance to another adjacent
territory. If the target territory belongs to the same player, then army units are moved to the target territory.
If the target territory belongs to another player, an attack is simulated when the order is executed. Once all
orders have been given by a player, they signify so to the game engine. When executing a deploy order, a number
of army units is added to a territory. A move is simply moving army units from one territory to another. An
attack is simulated by the following battle mechanism: First, the attacking player decides how many armies are
involved. Then, eac attacking army unit involved has a 60% chance of killing one defending army. At the same time,
each defending army unit has a 70% chance of killing one attacking army unit. If all the defender's armies are
eliminated, the attacker captures the territory. The attacking army units that survived the battle then occupy
the conquered territory. A player receives a card at the end of his turn if they successfully conquered at least
one territory during their turn. Cards can be played as the players are giving orders. Each card has a different
effect that influence the course of the game:

1. **Bomb**: The target country loses half of their army units.
2. **Reinforcement**: The player receives 5 reinforcement army units.
3. **Blockade**: The target territory's army units count is tripled, and the territory becomes neutral.
4. **Airlift**: Move any number of army units from one of your territories to another territory, even if they are not adjacent.
5. **Diplomacy**: Until the end of the turn, you and the target player cannot attack each other.

Any other player that does not control at least one territory is removed from the game. The game ends at any time one
of the players owns all the territories in the map.

# References
https://www.warzone.com/  
http://domination.sourceforge.net/getmaps.shtml
