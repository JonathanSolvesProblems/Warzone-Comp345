#include "Player.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

Player::Player(string name, int pID)
{
	playerName = name;
	playerID = pID;
	armees = 0;
	listOfOrders = new OrdersList();
	hand = new Hand();
}

int Player::getArmy()
{
	return army;
}

void Player::setArmy(int startingSoldiers)
{
	army = startingSoldiers;
}

void Player::addReinforcements(int soldiers)
{
	army += soldiers;
}

void Player::soldiersDied(int soldiers)
{
	army -= soldiers;
}

Player::Player(const Player &playerToCopy)
{
	this->playerName = string(playerToCopy.playerName);
	this->playerID = playerToCopy.playerID;
	this->armees = playerToCopy.getArmees();
	this->owned_territories = vector<map::Territory *>();
	this->owned_territories.assign(
			playerToCopy.owned_territories.begin(),
			playerToCopy.owned_territories.end());
	this->listOfOrders = new OrdersList(*(playerToCopy.listOfOrders));
	this->hand = new Hand(*(playerToCopy.hand));
}

int Player::getArmees() const
{
	return armees;
}

void Player::setArmees(int number)
{
	armees = number;
}

ostream &operator<<(ostream &out, const Player &playerToStream)
{
	out << playerToStream.playerName << " " << playerToStream.getArmees() ;
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

	owned_territories.clear();

#ifdef DEBUG
	cout << "Destructor completed for player" << endl;
#endif
}

const vector<map::Territory *> Player::toDefend()
{
	return owned_territories;
}

const vector<map::Territory *> Player::toAttack()
{
	unordered_set<map::Territory *> territories_to_attack = unordered_set<map::Territory *>();

	for (map::Territory *owned_territory : owned_territories)
	{
		for (map::Territory *neighbour : owned_territory->getNeighbours())
		{
			if (territories_to_attack.find(neighbour) != territories_to_attack.end() && neighbour->getOwner() != this)
			{
				territories_to_attack.insert(neighbour);
			}
		}
	}

	vector<map::Territory *> result = vector<map::Territory *>();
	result.assign(territories_to_attack.begin(), territories_to_attack.end());
	return result;
}

//Calls orderList's add method
void Player::issueOrder(Order *o)
{
	listOfOrders->add(o);
}

//Three cards get added to hand
void Player::draw(Deck &deck)
{
	Card *drawn = deck.draw();
	hand->add(drawn);
}

void Player::addTerritory(map::Territory *territory)
{
	if (territory)
	{
		this->notify();
		this->owned_territories.push_back(territory);
	}
}

void Player::removeTerritory(map::Territory *territory)
{
	if (territory)
	{
		std::remove(
				owned_territories.begin(),
				owned_territories.end(),
				territory);
	}
}

bool Player::isOwner(map::Territory *territory)
{
	if (territory)
	{
		for (map::Territory *owned : owned_territories)
		{
			if (territory == owned)
			{
				return true;
			}
		}
	}
	return false;
}
