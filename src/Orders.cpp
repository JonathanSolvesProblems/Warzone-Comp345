#include "Orders.h"

// --------------------------- Order Class ---------------------------
// Default constructor
Order::Order() : Order("This is a generic order.", "This order has no effect.") {
	// deliberately empty
}

// Parameterized constructor
Order::Order(string description, string effect) : _description(new string(description)), _effect(new string(effect)) {
	// deliberately empty
}

// Copy constructor
Order::Order(const Order& orderToCopy) {
	this->_description = new string(*(orderToCopy._description));
	this->_effect = new string(*(orderToCopy._effect));
}

// Destructor
Order::~Order() {
	delete _description;
	delete _effect;
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
// ----------------------- End of Order Class ------------------------

// ----------------------- AdvanceOrder Class ------------------------
// Default constructor
AdvanceOrder::AdvanceOrder() : Order("Advance Order", "Move some armies from one of the current player's territories (source) \
to an adjacent territory. If the target territory belongs to another player, then the armies are moved to the target territory. \
If the target territory belongs to another player, an attack happens between the two territories.") {
	// deliberately empty
}

// Parameterized constructor
AdvanceOrder::AdvanceOrder(map::Territory& sourceTerritory, map::Territory& targetTerritory, int numberOfArmies) : AdvanceOrder() {
	this->_sourceTerritory = new map::Territory(sourceTerritory);
	this->_targetTerritory = new map::Territory(targetTerritory);
	this->_numberOfArmies = new int(numberOfArmies);
}

// Copy constructor
AdvanceOrder::AdvanceOrder(const AdvanceOrder& advanceOrderToCopy) : Order(advanceOrderToCopy) {
	// deliberately empty
}

// Destructor
AdvanceOrder::~AdvanceOrder() {
	delete _sourceTerritory;
	delete _targetTerritory;
	delete _numberOfArmies;
}

// Checks whether the order is valid, and returns true if it is
bool AdvanceOrder::validate() {
	return true;
}

// Outputs the effect of the advance order and executes it
bool AdvanceOrder::execute() {
	if (validate()) {
		cout << *_effect << endl;
		return true;
	}
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
AirliftOrder::AirliftOrder() : Order("Airlift Order", "Advance some armies from one of the current player's territories to any other territory.") {
	// deliberately empty
}

// Parameterized constructor
AirliftOrder::AirliftOrder(map::Territory& sourceTerritory, map::Territory& targetTerritory, int numberOfArmies) : AirliftOrder() {
	this->_sourceTerritory = new map::Territory(sourceTerritory);
	this->_targetTerritory = new map::Territory(targetTerritory);
	this->_numberOfArmies = new int(numberOfArmies);
}

// Copy constructor
AirliftOrder::AirliftOrder(const AirliftOrder& airliftOrderToCopy) : Order(airliftOrderToCopy) {
	// deliberately empty
}

// Destructor
AirliftOrder::~AirliftOrder() {
	delete _sourceTerritory;
	delete _targetTerritory;
	delete _numberOfArmies;
}

// Checks whether the order is valid, and returns true if it is
bool AirliftOrder::validate() {
	return true;
}

// Outputs the effect of the airlift order and executes it
bool AirliftOrder::execute() {
	if (validate()) {
		cout << *_effect << endl;
		return true;
	}
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
BlockadeOrder::BlockadeOrder() : Order("Blockade Order", "Triple the number of armies on one of the current player's territories and make it a neutral territory.") {
	// deliberately empty
}

// Copy constructor
BlockadeOrder::BlockadeOrder(const BlockadeOrder& blockadeOrderToCopy) : Order(blockadeOrderToCopy) {
	// deliberately empty
}

// Destructor
BlockadeOrder::~BlockadeOrder() {
	// deliberately empty, default base class destructor will be called
}

// Checks whether the order is valid, and returns true if it is
bool BlockadeOrder::validate() {
	return true;
}

// Outputs the effect of the blockade order and executes it
bool BlockadeOrder::execute() {
	if (validate()) {
		cout << *_effect << endl;
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
BombOrder::BombOrder() : Order("Bomb Order", "Destroy half of the armies located on an opponent's territory that is adjacent to one of the current player's territories.") {
	// deliberately empty
}

// Copy constructor
BombOrder::BombOrder(const BombOrder& bombOrderToCopy) : Order(bombOrderToCopy) {
	// deliberately empty
}

// Destructor
BombOrder::~BombOrder() {
	// deliberately empty, default base class destructor will be called
}

// Checks whether the order is valid, and returns true if it is
bool BombOrder::validate() {
	return true;
}

// Outputs the effect of the bomb order and executes it
bool BombOrder::execute() {
	if (validate()) {
		cout << *_effect << endl;
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
DeployOrder::DeployOrder() : Order("Deploy Order", "Place some armies on one of the current player's territories.") {
	// deliberately empty
}

// Copy constructor
DeployOrder::DeployOrder(const DeployOrder& deployOrderToCopy) : Order(deployOrderToCopy) {
	// deliberately empty
}

// Destructor
DeployOrder::~DeployOrder() {
	// deliberately empty, default base class destructor will be called
}

// Checks whether the order is valid, and returns true if it is
bool DeployOrder::validate() {
	return true;
}

// Outputs the effect of the deploy order and executes it
bool DeployOrder::execute() {
	if (validate()) {
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
NegotiateOrder::NegotiateOrder() : Order("Negotiate Order", "Prevent attacks between the current player and another player until the end of the turn.") {
	// deliberately empty
}

// Copy constructor
NegotiateOrder::NegotiateOrder(const NegotiateOrder& negotiateOrderToCopy) : Order(negotiateOrderToCopy) {
	// deliberately empty
}

// Destructor
NegotiateOrder::~NegotiateOrder() {
	// deliberately empty, default base class destructor will be called
}

// Checks whether the order is valid, and returns true if it is
bool NegotiateOrder::validate() {
	return true;
}

// Outputs the effect of the negotiate order and executes it
bool NegotiateOrder::execute() {
	if (validate()) {
		cout << *_effect << endl;
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
}

// Copy constructor
OrdersList::OrdersList(const OrdersList &ordersListToCopy) {
	for (auto ordersIter = ordersListToCopy._orders.begin(); ordersIter != ordersListToCopy._orders.end(); ordersIter++) {
		_orders.push_back(*ordersIter);
	}
}

// Destructor
OrdersList::~OrdersList() {
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

// Overloads the stream insertion operator
ostream& operator<<(ostream& out, const OrdersList& ordersListToStream) {
	for (auto ordersIter = ordersListToStream._orders.begin(); ordersIter != ordersListToStream._orders.end(); ordersIter++) {
		out << **ordersIter << " ";
	}
	out << endl;
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
