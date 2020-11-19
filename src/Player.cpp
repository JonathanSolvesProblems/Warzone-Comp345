#include "Player.h"


Player::Player(string name, int pID)
{
	playerName = name;
	playerID = pID;
	armees = 0;
	listOfOrders = new OrdersList();
	hand = new Hand();
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
Order* Player::issueOrder()
{
	// return nullptr;
	if (listOfOrders->empty()) {
		_targetsThisRound = toAttack();
		_defencesThisRound = toDefend();
		_armees_to_deploy_this_round = armees;
	}

	if (_armees_to_deploy_this_round > 0)
	{
		return issueDeployOrder();
	}
	else if (hand->size() != 0) {
		
	}
	return nullptr;
}

Order* Player::nextOrder() {
	return listOfOrders->next();
}

Order* Player::issueAdvanceOrder() {
	srand(time(nullptr));

	map::Territory* targetTerritory = _targetsThisRound.front();
	_targetsThisRound.erase(_targetsThisRound.begin());
	vector<map::Territory*> sourceTerritories = targetTerritory->getNeighbours();

	map::Territory* sourceTerritory{nullptr};

	int offset = rand(); // Starting at a random offset, interate through neighbours to try and find a source territory which
											 // will result in a valid order
	for (int index = 0; index < sourceTerritories.size(); index++) {
		map::Territory *sourceTerritory = sourceTerritories.at((index + offset) % sourceTerritories.size());
		if (isOwner(sourceTerritory)) break;
	}

	int numberOfArmies = 0;
	if (sourceTerritory->getArmees() != 0) {
		numberOfArmies = rand() % sourceTerritory->getArmees() + 1;
	}

	AdvanceOrder* toReturn = new AdvanceOrder(*this, *sourceTerritory, *targetTerritory, numberOfArmies);
	listOfOrders->add(toReturn);

	return toReturn;
}

Order* Player::issueAirliftOrder() {
	srand(time(nullptr));

	map::Territory* targetTerritory = *(_targetsThisRound.begin());
	_targetsThisRound.erase(_targetsThisRound.begin());

	map::Territory* sourceTerritory = owned_territories.at(rand() % owned_territories.size());

	int numberOfArmies = 0;
	if (sourceTerritory->getArmees() != 0) {
		numberOfArmies = rand() % sourceTerritory->getArmees() + 1;
	}

	AirliftOrder* toReturn = new AirliftOrder(*this, *sourceTerritory, *targetTerritory, numberOfArmies);
	listOfOrders->add(toReturn);

	return toReturn;
}

Order* Player::issueBlockadeOrder() {
	map::Territory* nextDefence = *(_defencesThisRound.begin());
	_defencesThisRound.erase(_defencesThisRound.begin());

	BlockadeOrder* toReturn = new BlockadeOrder(*this, *nextDefence);
	listOfOrders->add(toReturn);

	return toReturn;
}

Order* Player::issueBombOrder() {
	//Bomb random enemy territory
	map::Territory* nextTarget = *(_targetsThisRound.begin());
	_targetsThisRound.pop_back();

	BombOrder* toReturn = new BombOrder(*this, *(nextTarget->getOwner()), *nextTarget);
	listOfOrders->add(toReturn);
	
	return toReturn;
}

Order* Player::issueDeployOrder()
{	
	map::Territory* nextDefence = *(_defencesThisRound.begin());
	_defencesThisRound.erase(_defencesThisRound.begin());

	int randomArmies = rand() % getArmees();

	if(randomArmies == 0){
		randomArmies += 1;
	}

	_armees_to_deploy_this_round -= randomArmies;
	DeployOrder *d = new DeployOrder(*this, *nextDefence, randomArmies);
	listOfOrders->add(d);

	return d;
}

Order* Player::issueNegotiateOrder()
{
	map::Territory* lastTarget = *(_targetsThisRound.end());

	NegotiateOrder* toReturn = new NegotiateOrder(*this, *(lastTarget->getOwner()));
	listOfOrders->add(toReturn);

	return toReturn;
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
