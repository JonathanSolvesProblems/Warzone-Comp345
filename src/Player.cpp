#include "Player.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


Player::Player(string name, int pID, vector<int> allCountries) {

	for (int s = 0; s < allCountries.size(); s++)
	{
		allTerritories.push_back(allCountries[s]);

	}
	//stores the player name
	playerName = name;
	//stores a player's ID
	playerID = pID;
	//
	
}

// Copy constructor
Player::Player(const Player& orderToCopy) {
	this->playerName = orderToCopy.playerName;
	this->playerID = orderToCopy.playerID;
	this->myTerritories = orderToCopy.myTerritories;
	this->allTerritories = orderToCopy.allTerritories;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const Player& playerToStream) {
	out << playerToStream.playerName;
	return out;
}

Player& Player::operator=(const Player& orderToAssign) {
	this->playerName = orderToAssign.playerName;
	this->playerID = orderToAssign.playerID;
	this->myTerritories = orderToAssign.myTerritories;
	this->allTerritories = orderToAssign.allTerritories;
	return *this;
}

Player::~Player() {
}

// maybe i should pass a map object to be able to determine what the terriotry actually is 
void Player::toDefend(map::Map test) {
	cout << "Territories To Defend" << "\n";

	for (int s = 0; s < myTerritories.size(); s++)
	{
		cout << *test.getTerritory(myTerritories[s]) << " in "<< test.getTerritory(myTerritories[s])->getContinent() << "\n";
	}
}

void Player::toAttack(map::Map test) {
	bool attack = false;
	cout << "Territories To Attack" << "\n";

	for (int s = 0; s < allTerritories.size(); s++)
	{
		//normally we would loop through and check the neighbours of each territory we posses and see if we owned the territory
		// take our myterritories vector and use the test (map) variable to get the neighbours in the vector and loop through 
		// we make another vector for territories to be attacked
		// upon doing this we have to chekc to see if we already own the territory so the if statement will follow the syntax bellow and get the neighbour vectors id
		// if the vector matches the second forloop (anything in our myterritories vector) then we skip and dont add it to the attack vector
		//else we add the id to the attack vector 
	
		// then we display the the attack vector 

		for (int x = 0; x < myTerritories.size(); x++)
		{

			if (allTerritories[s] == myTerritories[x]) {
				attack = true;

			}
		}

	

		if (attack) {
			attack = false;
		}

		else {
			cout << *test.getTerritory(allTerritories[s]) << " in " << test.getTerritory(allTerritories[s])->getContinent() << "\n";
			continue;
		}

	}

}

void Player::issueOrder(Order& o) {
	listOfOrders.add(&o);
	// creates a ordlist and adds it to the list of orders
}

void Player::playerHand(Hand& hand, Deck& deck) {

	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());

	hand.showHand();
}


