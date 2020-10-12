#pragma once
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
	// stores the ID's of the defending territories
	vector<int>myTerritories;
	// stores the ID's of all the territories we will be playing with 
	vector<int>allTerritories;
	// territories to attack
	vector<int>attack;
	//stores the player name
	std::string playerName;
	//stores a player's ID
	int playerID;
	
	//** may have to include a map object to figure out neighboruing territories 
	Player(std::string name, int pID, vector<int> allCountries);

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
};

