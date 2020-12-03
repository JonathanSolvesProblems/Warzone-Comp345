#include "Player.h"


Player::Player(string name, int pID)
{
	playerName = name;
	playerID = pID;
	armees = 0;
	listOfOrders = new OrdersList();
	hand = new Hand();
}

Player::Player(PlayerStrategy * initStrategy) {
	this->_strategy = initStrategy;
}

void Player::setStrategy(PlayerStrategy * newStrategy) {
	if (_strategy)
		delete _strategy;
	this->_strategy = newStrategy;
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
	delete _strategy;

	owned_territories.clear();

#ifdef DEBUG
	cout << "Destructor completed for player" << endl;
#endif
}

const vector<map::Territory *> Player::toDefend(GameModel *gm)
{
	return _strategy->toDefend(this, gm);
	// return owned_territories;
}

const vector<map::Territory *> Player::toAttack(GameModel *gm)
{
	return _strategy->toAttack(this, gm);
}

//Calls orderList's add method
Order* Player::issueOrder(GameModel* gm)
{
	if((this->listOfOrders)->empty()) {
		_strategy->beginRound(this,gm);
	}

	Order* order = _strategy->issueOrder(this, gm);
	
	if (order != nullptr){
		listOfOrders->add(order);
	}

	return order;
}

Order* Player::nextOrder() {
	return listOfOrders->next();
}

//Three cards get added to hand
void Player::draw(Deck &deck)
{
	Card *drawn = deck.draw();
	hand->add(drawn);
}

int Player::countCardsOfType(std::string t) {
	return hand->countCardsOfType(t);
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
		for (int i  = 0; i < owned_territories.size(); i++) {
			if (owned_territories.at(i) == territory) {
				owned_territories.erase(owned_territories.begin() + i);
				break;
			}
		}
		notify();
	}
}

bool Player::isOwner(map::Territory *territory)
{
	if (territory)
	{
		return territory->getOwner() == this;
	}
	return false;
}
