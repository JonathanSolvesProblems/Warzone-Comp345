#pragma once

class Player;

#include "Map.h"
#include "Card.h"
#include "Orders.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Player {
public:

	// an order list
	OrdersList* listOfOrders;
	// a vector of cards to hold the players hand
	Hand* hand;
	// stores the ID's of the defending territories
	vector<map::Territory*> owned_territories;

	//stores the player name
	std::string playerName;

	//stores a player's ID
	int playerID;
	
	/// <summary>
	/// Player constructor
	/// </summary>
	/// <param name="name">Name of player</param>
	/// <param name="pID">Player's identifier</param>
	/// <param name="allCountries">All Countries</param>
	Player(std::string name, int pID,);

	/// <summary>
	/// Copy Constructor
	/// </summary>
	/// <param name="orderToCopy">Copy order</param>
	Player(const Player& toCopy);

	/// <summary>
	/// Delete Player's only pointer field
	/// </summary>
	~Player();

	/// <summary>
	/// Outstream prints the player's name
	/// </summary>
	/// <param name="out"></param>
	/// <param name="playerToStream"></param>
	/// <returns></returns>
	friend ostream& operator<<(ostream& out, const Player& o);

	/// <summary>
	/// Assigns all fields of player to a new player
	/// </summary>
	/// <param name="playerToAssign"></param>
	/// <returns></returns>
	virtual Player& operator=(const Player& o);

	/// <summary>
	/// Draws three cards from a rhand and deck object
	/// </summary>
	/// <param name="hand">Hand object</param>
	/// <param name="deck">Deck object</param>
	void playerHand(Hand& hand, Deck& deck);

	/// <summary>
	/// Temporarily print map's territories that Player will defend
	/// </summary>
	/// <param name="test"></param>
	void toDefend(map::Map test);

	/// <summary>
	/// Set specific map's territories to be set to attack
	/// </summary>
	/// <param name="test">Map being interpreted</param>
	void toAttack(map::Map test);

	/// <summary>
	/// Adds order being passed to the player's list of orders
	/// </summary>
	/// <param name="o">Order being passed</param>
	void issueOrder(Order *o);

	// Adds a territory to myTerritories (Does not set the Territory's owner)
	void addTerritory(map::Territory*);

	// Removes a territory from myTerritories (Does not unset the Territory's owner)
	void removeTerritory(map::Territory*);
};

