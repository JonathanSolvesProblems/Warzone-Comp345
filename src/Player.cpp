#include "Player.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


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
	listOfOrders = new OrdersList();
}

Player::Player(const Player& orderToCopy) {
	this->playerName = orderToCopy.playerName;
	this->playerID = orderToCopy.playerID;
	this->myTerritories = orderToCopy.myTerritories;
	this->allTerritories = orderToCopy.allTerritories;
	this->listOfOrders = orderToCopy.listOfOrders;
}

ostream& operator<<(ostream& out, const Player& playerToStream) {
	out << playerToStream.playerName;
	return out;
}

Player& Player::operator=(const Player& playerToAssign) {
	this->playerName = playerToAssign.playerName;
	this->playerID = playerToAssign.playerID;
	this->myTerritories = playerToAssign.myTerritories;
	this->allTerritories = playerToAssign.allTerritories;
	this->listOfOrders = playerToAssign.listOfOrders;
	return *this;
}


Player::~Player() {
	delete listOfOrders;
	cout << "Destructor Called for player\n";
}

//Territories to defend are owned territories
void Player::toDefend(map::Map test) {
	cout << "Territories To Defend" << "\n";

	for (int s = 0; s < myTerritories.size(); s++)
	{
		cout << *test.getTerritory(myTerritories[s]) << " in "<< test.getTerritory(myTerritories[s])->getContinent() << "\n";
	}
}

//Territory index of owned is to be attacked
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

//Calls orderList's add method
void Player::issueOrder(Order* o) {
	listOfOrders->add(o);
}

//Three cards get added to hand
void Player::playerHand(Hand& hand, Deck& deck) {
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());

	hand.showHand();
}


