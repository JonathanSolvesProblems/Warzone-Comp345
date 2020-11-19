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
	if (listOfOrders->empty()) {
		_targetsThisRound = list<map::Territory*>(toAttack().begin(), toAttack().end());
		_defencesThisRound = list<map::Territory*>(toDefend().begin(), toDefend().end());
	}
	else if (armees > 0) {
		return issueDeployOrder();
	}
	else if (hand->size() != 0) {

	}
	else {
		return nullptr;
	}
}

Order* Player::issueAdvanceOrder()
{

  srand(time(nullptr));

  map::Territory* targetTerritory = *(_targetsThisRound.begin());
  _targetsThisRound.pop_front();

  map::Territory* sourceTerritory = owned_territories.at(rand() % owned_territories.size());
  //Get source territories neighbors

  std::vector<map::Territory *> neighbours = sourceTerritory->getNeighbours();


  int numberOfArmies = 0;
  if (sourceTerritory->getArmees() != 0) {
    numberOfArmies = rand() % sourceTerritory->getArmees() + 1;
  }

  player->issueOrder(new AdvanceOrder(*player, *sourceTerritory, *targetTerritory, numberOfArmies));
  _game_model->log->append("New Order issued: AdvcanceOrder");
}

Order* Player::issueDeployOrder()
{
  	srand(time(nullptr));
	
	map::Territory* nextDefence = *(_defencesThisRound.begin());
	_defencesThisRound.pop_front();

	int randomArmies = rand() % getArmees();

	if(randomArmies == 0){
		randomArmies += 1;
	}

	DeployOrder* d = new DeployOrder(*this, *nextDefence, randomArmies);
	listOfOrders->add(d);

	return d;
}

// void GameplayController::airliftSubPhase() {
//   srand(time(nullptr));
//   int numberOfTerritories = _game_model->map->getTerritories().size();
//   auto player = _game_model->current_player->get();

//   map::Territory* sourceTerritory = player->owned_territories.at(rand() % player->owned_territories.size());
//   map::Territory* targetTerritory = _game_model->map->getTerritory(rand() % numberOfTerritories);
//   int numberOfArmies = 0;
//   if (sourceTerritory->getArmees() != 0) {
//     numberOfArmies = rand() % sourceTerritory->getArmees() + 1;
//   }
//   player->issueOrder(new AirliftOrder(*player, *sourceTerritory, *targetTerritory, numberOfArmies));
//   _game_model->log->append("New Order issued: AirliftOrder");
// }

// void GameplayController::blockadeSubPhase() {
//   srand(time(nullptr));
//   auto player = _game_model->current_player->get();
//   map::Territory* targetTerritory = player->owned_territories.at(rand() % player->owned_territories.size());
//   player->issueOrder(new BlockadeOrder(*player, *targetTerritory));
//   _game_model->log->append("New Order issued: BlockadeOrder");
// }

// void GameplayController::bombSubPhase() {
//   //Bomb random enemy territory
  
//   std::vector<map::Territory *> allTerritories = _game_model->map->getTerritories();

//   std::vector<map::Territory *> ownedTerritories = _game_model->current_player->get()->owned_territories;

//   // allTerritories

// }

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
