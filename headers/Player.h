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
	// a order list
	OrdersList listOfOrders;

	// a vector of cards to hold the players hand
	Hand*
	// stores the ID's of the defending territories
	vector<map::Territory*> owned_territories;

	//stores the player name
	std::string playerName;

	//stores a player's ID
	int playerID;
	
	// Creates a new player with the given name and player id
	Player(std::string name, int pID);

	Player(const Player& toCopy);

	~Player();

	friend ostream& operator<<(ostream& out, const Player& o);
	virtual Player& operator=(const Player& o);

	void playerHand(Hand& hand, Deck& deck);

	// returns a series of strings of the territories to defend 
	void toDefend(map::Map test);

	// returns a series of stirng of the territories to attack 
	void toAttack(map::Map test);

	// creates a list object
	void issueOrder(Order& o);

	// Adds a territory to myTerritories (Does not set the Territory's owner)
	void addTerritory(map::Territory*);

	// Removes a territory from myTerritories (Does not unset the Territory's owner)
	void removeTerritory(map::Territory*);
};

