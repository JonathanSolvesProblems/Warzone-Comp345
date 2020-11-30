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
	// unordered_set<map::Territory *> territories_to_attack = unordered_set<map::Territory *>();

	// for (map::Territory *owned_territory : owned_territories)
	// {
	// 	float test = 1.0f / owned_territory->getNeighbours().size();
	// 	for (map::Territory *neighbour : owned_territory->getNeighbours())
	// 	{
	// 		if (territories_to_attack.find(neighbour) == territories_to_attack.end() && neighbour->getOwner() != this)
	// 		{
	// 			territories_to_attack.insert(neighbour);
	// 		}
	// 	}
	// }

	// vector<map::Territory *> result = vector<map::Territory *>();
	// result.assign(territories_to_attack.begin(), territories_to_attack.end());
	// return result;
}

//Calls orderList's add method
Order* Player::issueOrder(GameModel* gm)
{
	
	Order* order = _strategy->issueOrder(this, gm);
	
	if (order != nullptr){
		listOfOrders->add(order);
	}

	return order;
	// return nullptr;
	// if (listOfOrders->empty()) {
	// 	_targetsThisRound = toAttack();
	// 	_defencesThisRound = toDefend();
	// 	_armees_to_deploy_this_round = armees;
	// 	// hand->add(Deck::instance()->draw());
	// 	// hand->add(Deck::instance()->draw());
	// }

	// if (_armees_to_deploy_this_round > 0)
	// {
	// 	return issueDeployOrder();
	// }
	// else if (rand() % 10 > 3 && !_targetsThisRound.empty())
	// {
	// 	return issueAdvanceOrder();
	// }
	// else if (hand->size()) {
	// 	Card* cardToPlay = hand->playCard();
	// 	string order = cardToPlay->play();
	// 	Deck::instance()->add(cardToPlay);
	// 	if (order == "airlift" && (!_targetsThisRound.empty() || !_defencesThisRound.empty()))
	// 	{
	// 		return issueAirliftOrder();
	// 	}
	// 	else if (order == "bomb" && !_targetsThisRound.empty()) {
	// 		return issueBombOrder();
	// 	}
	// 	else if (order == "blockage" && !_defencesThisRound.empty()) {
	// 		return issueBlockadeOrder();
	// 	}
	// 	else if (order == "diplomacy" && !_targetsThisRound.empty())
	// 	{
	// 		return issueNegotiateOrder();
	// 	}
	// }
	// return nullptr;
}

Order* Player::nextOrder() {
	return listOfOrders->next();
}

// Order* Player::issueAdvanceOrder() {
// 	map::Territory *targetTerritory{nullptr};
// 	targetTerritory = _targetsThisRound.front();
// 	_targetsThisRound.erase(_targetsThisRound.begin());

// 	const vector<map::Territory*> &sourceTerritories = targetTerritory->getNeighbours();
// 	map::Territory* sourceTerritory{nullptr};

// 	for (map::Territory* t : sourceTerritories) {
// 		sourceTerritory = t;
// 		if (isOwner(sourceTerritory)) break;
// 	}

// 	int numberOfArmies = 0;
// 	if (sourceTerritory->getArmees() != 0) {
// 		numberOfArmies = rand() % sourceTerritory->getArmees() + 1;
// 	}

// 	AdvanceOrder* toReturn = new AdvanceOrder(*this, *sourceTerritory, *targetTerritory, numberOfArmies);
// 	listOfOrders->add(toReturn);

// 	return toReturn;
// }

// Order* Player::issueAirliftOrder() {
// 	map::Territory *targetTerritory{nullptr};
// 	if (!_targetsThisRound.empty())
// 	{
// 		targetTerritory = _targetsThisRound.front();
// 		_targetsThisRound.erase(_targetsThisRound.begin());
// 	}
// 	else
// 	{
// 		targetTerritory = _defencesThisRound.front();
// 		_targetsThisRound.erase(_defencesThisRound.begin());
// 	}

// 	map::Territory* sourceTerritory = owned_territories.at(rand() % owned_territories.size());

// 	int numberOfArmies = 0;
// 	if (sourceTerritory->getArmees() != 0) {
// 		numberOfArmies = rand() % sourceTerritory->getArmees() + 1;
// 	}

// 	AirliftOrder* toReturn = new AirliftOrder(*this, *sourceTerritory, *targetTerritory, numberOfArmies);
// 	listOfOrders->add(toReturn);

// 	return toReturn;
// }

// Order* Player::issueBlockadeOrder() {
// 	map::Territory* nextDefence = *(_defencesThisRound.begin());
// 	_defencesThisRound.erase(_defencesThisRound.begin());

// 	BlockadeOrder* toReturn = new BlockadeOrder(*this, *nextDefence);
// 	listOfOrders->add(toReturn);

// 	return toReturn;
// }

// Order* Player::issueBombOrder() {
// 	//Bomb random enemy territory
// 	map::Territory* nextTarget = _targetsThisRound.back();
// 	_targetsThisRound.pop_back();

// 	BombOrder* toReturn = new BombOrder(*this, *(nextTarget->getOwner()), *nextTarget);
// 	listOfOrders->add(toReturn);
	
// 	return toReturn;
// }

// Order* Player::issueDeployOrder()
// {	
// 	map::Territory* nextDefence = *(_defencesThisRound.begin());
// 	_defencesThisRound.erase(_defencesThisRound.begin());

// 	int randomArmies = rand() % _armees_to_deploy_this_round;

// 	if(randomArmies == 0){
// 		randomArmies += 1;
// 	}

// 	_armees_to_deploy_this_round -= randomArmies;
// 	DeployOrder *d = new DeployOrder(*this, *nextDefence, randomArmies);
// 	listOfOrders->add(d);

// 	return d;
// }

// Order* Player::issueNegotiateOrder()
// {
// 	map::Territory* lastTarget = _targetsThisRound.back();

// 	NegotiateOrder* toReturn = new NegotiateOrder(*this, *(lastTarget->getOwner()));
// 	listOfOrders->add(toReturn);

// 	return toReturn;
// }

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
