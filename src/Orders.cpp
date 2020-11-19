#include "../headers/Orders.h"

// --------------------------- Order Class ---------------------------
// Default constructor
Order::Order() : Order("This is a generic order.", "Not Yet Executed") {
	// deliberately empty
}

// Parameterized constructor
Order::Order(string description, string effect) : _description(new string(description)), _effect(new string(effect)) {
	// deliberately emptys
}

// Copy constructor
Order::Order(const Order& orderToCopy) {
	this->_description = new string(*(orderToCopy._description));
	this->_effect = new string(*(orderToCopy._effect));
	this->_issuingPlayer = orderToCopy._issuingPlayer;
}

// Destructor
Order::~Order() {
	delete _description;
	delete _effect;
}

string Order::toString() {
	return *_description;
}

string Order::getEffect() {
	return *_effect;
}

int Order::getPriority() {
	return priority;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const Order& orderToStream) {
	out << *(orderToStream._description);
	return out;
}

// Overloads the assignment operator.
Order& Order::operator=(const Order& orderToAssign) {
	this->_description = new string(*(orderToAssign._description));
	this->_effect = new string(*(orderToAssign._effect));
	return *this;
}

bool Order::checkIfTruce(Player* _issuingPlayer, Player* _targetPlayer) {
	for (int i = 0; i < truces.size();i++) {
		//cout << "truce count: " << *truceIter << endl;

		tuple<Player*,Player*> truce = truces.at(i);
		Player* p1 = std::get<0>(truce);
		Player* p2 = std::get<1>(truce);

		if( p1 ==  _issuingPlayer && p2 == _targetPlayer ) {
			return true;
		}
		if( p1 == _targetPlayer && p2 == _issuingPlayer) {
			return true;
		}
	}
	return false;
}
// ----------------------- End of Order Class ------------------------

// ----------------------- AdvanceOrder Class ------------------------
// Default constructor
AdvanceOrder::AdvanceOrder() : Order("Advance Order", "Move some armies from one of the current player's territories (source) \
to an adjacent territory. If the target territory belongs to another player, then the armies are moved to the target territory. \
If the target territory belongs to another player, an attack happens between the two territories.") {
	// deliberately empty
}

// Parameterized constructor
AdvanceOrder::AdvanceOrder(Player& issuingPlayer, map::Territory& sourceTerritory, map::Territory& targetTerritory, int numberOfArmies) : AdvanceOrder() {
	this->_issuingPlayer = &issuingPlayer;
	this->_sourceTerritory = &sourceTerritory;
	this->_targetTerritory = &targetTerritory;
	this->_numberOfArmies = numberOfArmies;
}

// Copy constructor
AdvanceOrder::AdvanceOrder(const AdvanceOrder& advanceOrderToCopy) : Order(advanceOrderToCopy) {
	// deliberately empty
}

// Destructor
AdvanceOrder::~AdvanceOrder() {
	// deliberately empty, Player and Territory pointers will be dealt with in their own scope

}

// Checks whether the order is valid, and returns true if it is
bool AdvanceOrder::validate() {

	map::Territory* ptr = _sourceTerritory->getNeighbour(_targetTerritory->getName());
	
	if(ptr == nullptr)
		return false;

		
	if(_sourceTerritory->getOwner() == _issuingPlayer && _numberOfArmies <= _issuingPlayer->getArmees()) {
		if(checkIfTruce(_issuingPlayer,_targetTerritory->getOwner())){
			return false;
		}
		return true;
	}

	return false;
}

// Outputs the effect of the advance order and executes it
bool AdvanceOrder::execute() {
	if (validate()) {
		if (_sourceTerritory->getArmees() < _numberOfArmies)
			_numberOfArmies = _sourceTerritory->getArmees();
		if(_sourceTerritory->getOwner() == _targetTerritory->getOwner()) {
			//Remove from source and add to target
			this->_sourceTerritory->removeArmees(this->_numberOfArmies);
			this->_targetTerritory->addArmees(this->_numberOfArmies);
			*_effect = "moved " + std::to_string(_numberOfArmies) + " from " + this->_sourceTerritory->getName() + " to " + this->_targetTerritory->getName();
			return true;
		}
		else {
			srand(time(0));
			int troopsLost = 0; // source armies
			int enemiesKilled = 0; // target armies

			for(int i = 0; i < this->_numberOfArmies ; i++) {
				int attackingOdds = rand() % 10 + 1;
				if(attackingOdds <= 6) {
					enemiesKilled++;
					if(enemiesKilled == this->_targetTerritory->getArmees() ) {
						break;
					}
				}
			}

			for(int i = 0; i < this->_targetTerritory->getArmees() ; i++) {
				int defendingOdds = rand() % 10 + 1;
				if(defendingOdds <= 7) {
					troopsLost++;
					if(troopsLost == this->_numberOfArmies ) {
						break;
					}
				}
			}
			//All enemies are dead, and you still have armies left
			if(enemiesKilled >= this->_targetTerritory->getArmees() && troopsLost < this->_numberOfArmies ){
				this->_issuingPlayer->draw(*(Deck::instance()));
				//change ownership to issuingPlayer
				this->_targetTerritory->setOwner(this->_issuingPlayer);
				//Change armies values
				this->_sourceTerritory->removeArmees(_numberOfArmies);
				this->_targetTerritory->setArmees( this->_numberOfArmies - troopsLost);
				*_effect = "successfully invaded " + this->_targetTerritory->getName() + " from " + this->_sourceTerritory->getName() + " with " + std::to_string(this->_numberOfArmies - troopsLost) + " armees";
			}

			//All your troops are dead, and your enemy has troops left
			else if(troopsLost >= this->_numberOfArmies && enemiesKilled < this->_targetTerritory->getArmees() ){
				//Change armies values
				this->_sourceTerritory->setArmees( this->_sourceTerritory->getArmees() - troopsLost);
				this->_targetTerritory->setArmees( this->_targetTerritory->getArmees() - enemiesKilled);
				*_effect = "failed invasion to" + this->_targetTerritory->getName() + " from " + this->_sourceTerritory->getName();
			}
		}
	}
	
	*_effect = "REJECTED";
	return false;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const AdvanceOrder& advanceOrderToStream) {
	out << static_cast <const Order&>(advanceOrderToStream); // upcast to Order to call their stream insertion operator
	return out;
}

// Overloads the assignment operator.
AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& advanceOrderToAssign) {
	Order::operator= (advanceOrderToAssign);
	return *this;
}
// ------------------- End of AdvanceOrder Class ---------------------

// ----------------------- AirliftOrder Class ------------------------
// Default constructor
AirliftOrder::AirliftOrder() : Order("Airlift Order", "Not Yet Executed")
{
	// deliberately empty
}

// Parameterized constructor
AirliftOrder::AirliftOrder(Player& issuingPlayer, map::Territory& sourceTerritory, map::Territory& targetTerritory, int numberOfArmies) : AirliftOrder() {
	this->_issuingPlayer = &issuingPlayer;
	this->_sourceTerritory = &sourceTerritory;
	this->_targetTerritory = &targetTerritory;
	this->_numberOfArmies = numberOfArmies;
}

// Copy constructor
AirliftOrder::AirliftOrder(const AirliftOrder& airliftOrderToCopy) : Order(airliftOrderToCopy) {
	// deliberately empty
}

// Destructor
AirliftOrder::~AirliftOrder() {
	// deliberately empty, Player and Territory pointers will be dealt with in their own scope
}

// Checks whether the order is valid, and returns true if it is
bool AirliftOrder::validate() {
	
	//Check that the source and target belongs to the player that issued the order
	if(_sourceTerritory->getOwner() == _issuingPlayer && _numberOfArmies <= _issuingPlayer->getArmees()) {
		if(checkIfTruce(_issuingPlayer,_targetTerritory->getOwner())) {
			return false;
		}
		return true;
	}
	return false;
}

// Outputs the effect of the airlift order and executes it
bool AirliftOrder::execute() {
	if (validate()) {
		if (_sourceTerritory->getArmees() < _numberOfArmies)
			_numberOfArmies = _sourceTerritory->getArmees();
		if(_sourceTerritory->getOwner() == _targetTerritory->getOwner()) {
			//Remove from source and add to target
			this->_sourceTerritory->removeArmees(this->_numberOfArmies);
			this->_targetTerritory->addArmees(this->_numberOfArmies);
			_issuingPlayer->setArmees(_issuingPlayer->getArmees() - _numberOfArmies);
			*_effect = "moved " + std::to_string(_numberOfArmies) + " from " + this->_sourceTerritory->getName() + " to " + this->_targetTerritory->getName();
			return true;
		}
		else {
			srand(time(0));
			int troopsLost = 0;
			int enemiesKilled = 0;

			for(int i = 0; i < this->_numberOfArmies ; i++) {
				int attackingOdds = rand() % 10 + 1;
				if(attackingOdds <= 6) {
					enemiesKilled++;
					if(enemiesKilled == this->_targetTerritory->getArmees() ) {
						break;
					}
				}
			}

			for(int i = 0; i < this->_targetTerritory->getArmees() ; i++) {
				int defendingOdds = rand() % 10 + 1;
				if(defendingOdds <= 7) {
					troopsLost++;
					if(troopsLost == this->_numberOfArmies ) {
						break;
					}
				}
			}
			//All enemies are dead, and you still have armies left
			if(enemiesKilled >= this->_targetTerritory->getArmees() && troopsLost < this->_numberOfArmies ){
				//Draw card
				this->_issuingPlayer->draw(*(Deck::instance()));
				//change ownership to issuingPlayer
				this->_targetTerritory->setOwner(this->_issuingPlayer);
				//Change armies values
				this->_sourceTerritory->removeArmees(_numberOfArmies);
				this->_targetTerritory->setArmees( this->_numberOfArmies - troopsLost);

				*_effect = "successfully invaded " + this->_targetTerritory->getName() + " from " + this->_sourceTerritory->getName() + " with " + std::to_string(this->_numberOfArmies - troopsLost) + " armees";
			}

			//All your troops are dead, and your enemy has troops left
			else if(troopsLost >= this->_numberOfArmies && enemiesKilled < this->_targetTerritory->getArmees() ){
				//Change armies values
				this->_sourceTerritory->setArmees( this->_sourceTerritory->getArmees() - troopsLost);
				this->_targetTerritory->setArmees( this->_targetTerritory->getArmees() - enemiesKilled);

				*_effect = "failed invasion to" + this->_targetTerritory->getName() + " from " + this->_sourceTerritory->getName();
			}
		}
		return true;
	}

	*_effect = "REJECTED";
	return false;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const AirliftOrder& airliftOrderToStream) {
	out << static_cast <const Order&>(airliftOrderToStream); // upcast to Order to call their stream insertion operator
	return out;
}

// Overloads the assignment operator.
AirliftOrder& AirliftOrder::operator=(const AirliftOrder& airliftOrderToAssign) {
	Order::operator= (airliftOrderToAssign);
	return *this;
}
// ------------------- End of AirliftOrder Class ---------------------

// ---------------------- BlockadeOrder Class ------------------------
// Default constructor
BlockadeOrder::BlockadeOrder() : Order("Blockade Order", "Not Yet Executed")
{
	// deliberately empty
}

// Parameterized constructor
BlockadeOrder::BlockadeOrder(Player& issuingPlayer, map::Territory& targetTerritory) : BlockadeOrder() {
	this->_issuingPlayer = &issuingPlayer;
	this->_targetTerritory = &targetTerritory;
}

// Copy constructor
BlockadeOrder::BlockadeOrder(const BlockadeOrder& blockadeOrderToCopy) : Order(blockadeOrderToCopy) {
	// deliberately empty
}

// Destructor
BlockadeOrder::~BlockadeOrder() {
	// deliberately empty, Player and Territory pointers will be dealt with in their own scope
}

// Checks whether the order is valid, and returns true if it is
bool BlockadeOrder::validate() {
	//Target doesn't belong to an enemy player
	//TODO: Ask about checking for enemy players
	if(_targetTerritory->getOwner() == _issuingPlayer)
		return true;
	return false;
}

// Outputs the effect of the blockade order and executes it
bool BlockadeOrder::execute() {
	if (validate()) {

		//Double armies
		this->_targetTerritory->addArmees(this->_targetTerritory->getArmees());



		this->_issuingPlayer->removeTerritory(_targetTerritory);

		//Set neutral player owner
		this->_targetTerritory->setOwner(nullptr);

		return true;
	}
	return false;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const BlockadeOrder& blockadeOrderToStream) {
	out << static_cast <const Order&>(blockadeOrderToStream); // upcast to Order to call their stream insertion operator
	return out;
}

// Overloads the assignment operator.
BlockadeOrder& BlockadeOrder::operator=(const BlockadeOrder& blockadeOrderToAssign) {
	Order::operator= (blockadeOrderToAssign);
	return *this;
}
// ------------------ End of BlockadeOrder Class ---------------------

// ------------------------ BombOrder Class --------------------------
// Default constructor
BombOrder::BombOrder() : Order("Bomb Order", "Not Yet Executed")
{
	// deliberately empty
}

// Parameterized constructor
BombOrder::BombOrder(Player& issuingPlayer, Player& targetPlayer, map::Territory& targetTerritory) : BombOrder() {
	this->_issuingPlayer = &issuingPlayer;
	this->_targetPlayer = &targetPlayer;
	this->_targetTerritory = &targetTerritory;
}

// Copy constructor
BombOrder::BombOrder(const BombOrder& bombOrderToCopy) : Order(bombOrderToCopy) {
	// deliberately empty
}

// Destructor
BombOrder::~BombOrder() {
	// deliberately empty, Player and Territory pointers will be dealt with in their own scope
}

// Checks whether the order is valid, and returns true if it is
bool BombOrder::validate() {

	//Check that target doesn't belong to player that issued order
	if(_targetTerritory->getOwner() == _issuingPlayer)
		return false;

	if(checkIfTruce(_issuingPlayer,_targetPlayer)) {
		return false;
	}
	return true;
}

// Outputs the effect of the bomb order and executes it
bool BombOrder::execute() {
	if (validate()) {
		//Territory being bombed belongs to enemy player, half of the armies get removed
		_targetTerritory->removeArmees(ceil(_targetTerritory->getArmees() / 2.0));
		//cout << *_effect << endl;
		return true;
	}
	return false;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const BombOrder& bombOrderToStream) {
	out << static_cast <const Order&>(bombOrderToStream); // upcast to Order to call their stream insertion operator
	return out;
}

// Overloads the assignment operator.
BombOrder& BombOrder::operator=(const BombOrder& bombOrderToAssign) {
	Order::operator= (bombOrderToAssign);
	return *this;
}
// -------------------- End of BombOrder Class -----------------------

// ------------------------ DeployOrder Class ------------------------
// Default constructor
DeployOrder::DeployOrder() : Order("Deploy Order", "Not Yet Executed")
{
	// deliberately empty
}

// Parameterized constructor
DeployOrder::DeployOrder(Player& issuingPlayer, map::Territory& targetTerritory, int numberOfArmies) : DeployOrder() {
	this->_issuingPlayer = &issuingPlayer;
	this->_targetTerritory = &targetTerritory;
	this->_numberOfArmies = numberOfArmies;
}

// Copy constructor
DeployOrder::DeployOrder(const DeployOrder& deployOrderToCopy) : Order(deployOrderToCopy) {
	// deliberately empty
}

// Destructor
DeployOrder::~DeployOrder() {
	// deliberately empty, Player and Territory pointers will be dealt with in their own scope
}

// Checks whether the order is valid, and returns true if it is
bool DeployOrder::validate() {
	//If target targetTerritory belongs to player, return true
	if(_targetTerritory->getOwner() == _issuingPlayer && _numberOfArmies <= _issuingPlayer->getArmees())
		return true;
	return false;
}

// Outputs the effect of the deploy order and executes it
bool DeployOrder::execute() {
	if (validate()) {

		//Add passed number of armees to target territory
		this->_targetTerritory->addArmees(this->_numberOfArmies);

		_issuingPlayer->setArmees(_issuingPlayer->getArmees() - _numberOfArmies);


		cout << *_effect << endl;
		return true;
	}
	return false;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const DeployOrder& deployOrderToStream) {
	out << static_cast <const Order&>(deployOrderToStream); // upcast to Order to call their stream insertion operator
	return out;
}

// Overloads the assignment operator.
DeployOrder& DeployOrder::operator=(const DeployOrder& deployOrderToAssign) {
	Order::operator= (deployOrderToAssign);
	return *this;
}
// -------------------- End of DeployOrder Class ---------------------

// --------------------- NegotiateOrder Class ------------------------
// Default constructor
NegotiateOrder::NegotiateOrder() : Order("Negotiate Order", "Not Yet Executed") {
	// deliberately empty
}

// Parameterized constructor
NegotiateOrder::NegotiateOrder(Player& issuingPlayer, Player& secondPlayer) : NegotiateOrder() {
	this->_issuingPlayer = &issuingPlayer;
	this->_secondPlayer = &secondPlayer;
}

// Copy constructor
NegotiateOrder::NegotiateOrder(const NegotiateOrder& negotiateOrderToCopy) : Order(negotiateOrderToCopy) {
	// deliberately empty
}

// Destructor
NegotiateOrder::~NegotiateOrder() {
	// deliberately empty, Player pointers will be dealt with in their own scope
}

// Checks whether the order is valid, and returns true if it is
bool NegotiateOrder::validate() {
	//target cannot be the same player issuing the order
	if(_issuingPlayer == _secondPlayer)
		return false;
	return true;
}

bool NegotiateOrder::execute() {
	if (validate()) {
		tuple<Player*,Player*> truce;

		std::get<0>(truce) = _issuingPlayer;
		std::get<1>(truce) = _secondPlayer;

		truces.push_back(truce);
		return true;
	}
	return false;
}

// Overloads the stream insertion operator.
ostream& operator<<(ostream& out, const NegotiateOrder& negotiateOrderToStream) {
	out << static_cast <const Order&>(negotiateOrderToStream); // upcast to Order to call their stream insertion operator
	return out;
}

// Overloads the assignment operator.
NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& negotiateOrderToAssign) {
	Order::operator= (negotiateOrderToAssign);
	return *this;
}
// ----------------- End of NegotiateOrder Class ---------------------

// ------------------------ OrdersList Class -------------------------
// Default constructor
OrdersList::OrdersList() {
	_orders;
	truces;
}

// Copy constructor
OrdersList::OrdersList(const OrdersList &ordersListToCopy) {
	for (auto ordersIter = ordersListToCopy._orders.begin(); ordersIter != ordersListToCopy._orders.end(); ordersIter++) {
		_orders.push_back(*ordersIter);
	}
}

// Destructor
OrdersList::~OrdersList() {
	for (Order* order : _orders) {
		delete order;
	}
	_orders.clear();
}

// Takes a pointer to an order and adds it to the orders list
void OrdersList::add(Order* orderToAdd) {
	_orders.push_back(orderToAdd);
}

// Moves an order in the list of orders
void OrdersList::move(int oldIndex, int newIndex) {
	auto oldPosition = _orders.begin();
	advance(oldPosition, oldIndex);
	auto newPosition = _orders.begin();
	advance(newPosition, newIndex);
	_orders.splice(newPosition, _orders, oldPosition);
}

// Removes an item from the order list
void OrdersList::remove(int index) {
	int position = 0;
	for (auto ordersIter = _orders.begin(); ordersIter != _orders.end(); ordersIter++) {
		if (position == index) {
			_orders.erase(ordersIter);
			return;
		}
		position++;
	}
}

Order *OrdersList::next()
{
	std::stable_sort(_orders.begin(), _orders.end(), [](Order *a, Order *b) {
		return a->getPriority() < b->getPriority();
	});
	Order* next_order = _orders.front();
	_orders.pop_front();
	return next_order;
}

bool OrdersList::empty() {
	return _orders.empty();
}

// Overloads the stream insertion operator
ostream& operator<<(ostream& out, const OrdersList& ordersListToStream) {
	for (auto ordersIter = ordersListToStream._orders.begin(); ordersIter != ordersListToStream._orders.end(); ordersIter++) {
		out << **ordersIter << " ";
	}
	out << endl;
	return out;
}

string OrdersList::toString() {
	string out = "";
	for (auto ordersIter : _orders) {
		out += ordersIter->toString() + " ";
	}
	return out;
}

// Overloads the assignment operator
OrdersList& OrdersList::operator=(OrdersList& ordersListToAssign) {
	for (auto ordersIter = ordersListToAssign._orders.begin(); ordersIter != ordersListToAssign._orders.end(); ordersIter++) {
		_orders.push_back(*ordersIter);
	}
	return *this;
}
// --------------------- End of OrdersList Class ---------------------
