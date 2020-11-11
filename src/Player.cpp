#include "Player.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

Player::Player(string name, int pID) {
	playerName = name;
	playerID = pID;
	listOfOrders = new OrdersList();
	hand = new Hand();
}

Player::Player(const Player& playerToCopy) {
	this->playerName = string(playerToCopy.playerName);
	this->playerID = playerToCopy.playerID;
	this->owned_territories = vector<map::Territory *>();
	this->owned_territories.assign(
			playerToCopy.owned_territories.begin(),
			playerToCopy.owned_territories.end());
	this->listOfOrders = new OrdersList(*(playerToCopy.listOfOrders));
	this->hand = new Hand(*(playerToCopy.hand));
}

ostream &operator<<(ostream &out, const Player &playerToStream)
{
	out << playerToStream.playerName;
	return out;
}

Player Player::operator=(const Player &playerToAssign)
{
		return Player(playerToAssign);
}

Player::~Player()
{
	delete hand;
	delete listOfOrders;

	for (map::Territory* territory : owned_territories) {
		if (territory)
			territory->setOwner(nullptr);
	}
	owned_territories.clear();

#ifdef DEBUG
	cout << "Destructor completed for player" << endl;
#endif
}

const vector<map::Territory*> Player::toDefend() {
	return owned_territories;
}

const vector<map::Territory*> Player::toAttack() {
	unordered_set<map::Territory *> territories_to_attack = unordered_set<map::Territory *>();
	
	for (map::Territory* owned_territory : owned_territories) {
		for (map::Territory* neighbour : owned_territory->getNeighbours()) {
			if (territories_to_attack.find(neighbour) != territories_to_attack.end() && neighbour->getOwner() != this) {
				territories_to_attack.insert(neighbour);
			}
		}
	}

	vector<map::Territory*> result = vector<map::Territory*>();
	result.assign(territories_to_attack.begin(), territories_to_attack.end());
	return result;
}

//Calls orderList's add method
void Player::issueOrder(Order* o) {
	listOfOrders->add(o);
}

//Three cards get added to hand
void Player::draw(Deck& deck) {
	Card* drawn = deck.draw();
	hand->add(drawn);
}

void Player::addTerritory(map::Territory* territory) {
	if (territory) {
		this->owned_territories.push_back(territory);
	}
}

void Player::removeTerritory(map::Territory* territory) {
	if (territory) {
		std::remove(
			owned_territories.begin(),
			owned_territories.end(),
			territory
		);
	}
}

bool Player::isOwner(map::Territory* territory) {
	if(territory){
		for (map::Territory* owned : owned_territories) {
			if(territory == owned){
				return true;
			}
		}
	}
	return false;
}



