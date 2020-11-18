#pragma once

class Order;
class OrdersList;

#include <cmath>
#include <iostream>
#include <list>
#include "Map.h"
#include "Player.h"
#include <tuple>

using std::ostream;
using std::string;
using std::list;
using std::endl; 
using std::tuple;

static vector<tuple<Player*,Player*>> truces;

/// <summary>
/// Order Class
/// </summary>
/// <remarks>
/// You can validate and execute derived orders.
/// </remarks>
class Order {
public:
	// Constructors
	Order();
	Order(string description, string effect);
	Order(const Order& orderToCopy);
	// Destructor
	~Order();
	// Service methods
	/// <summary>
	/// Validates if the order is valid.
	/// </summary>
	/// <returns>
	/// Whether the order is valid or not.
	/// </returns>
	virtual bool validate() = 0;
	/// <summary>
	/// Validates the order, and executes it if it is valid and outputs the effect.
	/// </summary>
	/// <returns>
	/// Whether the order successfully executed or not.
	/// </returns>
	virtual bool execute() = 0;

	string toString();

	bool checkIfTruce(Player* _issuingPlayer, Player* _targetPlayer);
	/// <summary>
	/// Sends the order description to the output stream.
	/// </summary>
	friend ostream& operator<<(ostream& out, const Order& o);
	/// <summary>
	/// Assigns a copy of the order description and effect to another order variable.
	/// </summary>
	virtual Order& operator=(const Order& o);
protected:
	// Data members
	string* _description{nullptr};
	string* _effect{nullptr};
	Player* _issuingPlayer{nullptr};
};

/// <summary>
/// Advance Order Class
/// </summary>
/// <remarks>
/// Can be validated and when executed, will advance armies to a specified territory.
/// </remarks>
class AdvanceOrder : public Order {
public:
	// Constructors
	AdvanceOrder();
	AdvanceOrder(Player& issuingPlayer, map::Territory& sourceTerritory, map::Territory& targetTerritory, int numberOfArmies);
	AdvanceOrder(const AdvanceOrder& orderToCopy);
	// Destructor
	~AdvanceOrder();
	// Service methods
	/// <summary>
	/// Validates if the advance order is valid.
	/// </summary>
	/// <returns>
	/// Whether the advance order is valid or not.
	/// </returns>
	bool validate() override final;
	/// <summary>
	/// Validates the advance order, and executes it if it is valid and outputs the effect.
	/// </summary>
	/// <returns>
	/// Whether the advance order successfully executed or not.
	/// </returns>
	bool execute() override final;
	/// <summary>
	/// Sends the advance order description to the output stream.
	/// </summary>
	friend ostream& operator<<(ostream& out, const AdvanceOrder& o);
	/// <summary>
	/// Assigns a copy of the advance order description and effect to another advance order variable.
	/// </summary>
	AdvanceOrder& operator=(const AdvanceOrder& o);
private:
	// Data members
	map::Territory* _sourceTerritory{nullptr};
	map::Territory* _targetTerritory{nullptr};
	int _numberOfArmies{0};
};

/// <summary>
/// Airlift Order Class
/// </summary>
/// <remarks>
/// Can be validated and when executed, will airlift armies to a specified territory.
/// </remarks>
class AirliftOrder : public Order {
public:
	// Constructors
	AirliftOrder();
	AirliftOrder(Player& issuingPlayer, map::Territory& sourceTerritory, map::Territory& targetTerritory, int numberOfArmies);
	AirliftOrder(const AirliftOrder& orderToCopy);
	// Destructor
	~AirliftOrder();
	// Service methods
	/// <summary>
	/// Validates if the airlift order is valid.
	/// </summary>
	/// <returns>
	/// Whether the airlift order is valid or not.
	/// </returns>
	bool validate() override final;
	/// <summary>
	/// Validates the airlift order, and executes it if it is valid and outputs the effect.
	/// </summary>
	/// <returns>
	/// Whether the airlift order successfully executed or not.
	/// </returns>
	bool execute() override final;
	/// <summary>
	/// Sends the airlift order description to the output stream.
	/// </summary>
	friend ostream& operator<<(ostream& out, const AirliftOrder& o);
	/// <summary>
	/// Assigns a copy of the airlift order description and effect to another airlift order variable.
	/// </summary>
	AirliftOrder& operator=(const AirliftOrder& o);
private:
	// Data members
	map::Territory* _sourceTerritory{nullptr};
	map::Territory* _targetTerritory{nullptr};
	int _numberOfArmies{0};
};

/// <summary>
/// Blockade Order Class
/// </summary>
/// <remarks>
/// Can be validated and when executed, will triple the armies in a specified territory.
/// </remarks>
class BlockadeOrder : public Order {
public:
	// Constructors
	BlockadeOrder();
	BlockadeOrder(Player& issuingPlayer, map::Territory& targetTerritory);
	BlockadeOrder(const BlockadeOrder& orderToCopy);
	// Destructor
	~BlockadeOrder();
	// Service methods
	/// <summary>
	/// Validates if the blockade order is valid.
	/// </summary>
	/// <returns>
	/// Whether the blockade order is valid or not.
	/// </returns>
	bool validate() override final;
	/// <summary>
	/// Validates the blockade order, and executes it if it is valid and outputs the effect.
	/// </summary>
	/// <returns>
	/// Whether the blockade order successfully executed or not.
	/// </returns>
	bool execute() override final;
	/// <summary>
	/// Sends the blockade order description to the output stream.
	/// </summary>
	friend ostream& operator<<(ostream& out, const BlockadeOrder& o);
	/// <summary>
	/// Assigns a copy of the blockade order description and effect to another blockade order variable.
	/// </summary>
	BlockadeOrder& operator=(const BlockadeOrder& o);
private:
	// Data members
	map::Territory* _targetTerritory{nullptr};
};

/// <summary>
/// Bomb Order Class
/// </summary>
/// <remarks>
/// Can be validated and when executed, will destroy half the armies in a specified territory.
/// </remarks>
class BombOrder : public Order {
public:
	// Constructors
	BombOrder();
	BombOrder(Player& issuingPlayer, Player& targetPlayer, map::Territory& targetTerritory);
	BombOrder(const BombOrder& orderToCopy);
	// Destructor
	~BombOrder();
	// Service methods
	/// <summary>
	/// Validates if the bomb order is valid.
	/// </summary>
	/// <returns>
	/// Whether the bomb order is valid or not.
	/// </returns>
	bool validate() override final;
	/// <summary>
	/// Validates the bomb order, and executes it if it is valid and outputs the effect.
	/// </summary>
	/// <returns>
	/// Whether the bomb order successfully executed or not.
	/// </returns>
	bool execute() override final;
	/// <summary>
	/// Sends the bomb order description to the output stream.
	/// </summary>
	friend ostream& operator<<(ostream& out, const BombOrder& o);
	/// <summary>
	/// Assigns a copy of the bomb order description and effect to another bomb order variable.
	/// </summary>
	BombOrder& operator=(const BombOrder& o);
private:
	// Data members
	Player* _targetPlayer{nullptr};
	map::Territory* _targetTerritory{nullptr};
};

/// <summary>
/// Deploy Order Class
/// </summary>
/// <remarks>
/// Can be validated and when executed, will place some armies in a specified territory.
/// </remarks>
class DeployOrder : public Order {
public:
	// Constructors
	DeployOrder();
	DeployOrder(Player& issuingPlayer, map::Territory& targetTerritory, int _numberOfArmies);
	DeployOrder(const DeployOrder& orderToCopy);
	// Destructor
	~DeployOrder();
	// Service methods
	/// <summary>
	/// Validates if the deploy order is valid.
	/// </summary>
	/// <returns>
	/// Whether the deploy order is valid or not.
	/// </returns>
	bool validate() override final;
	/// <summary>
	/// Validates the deploy order, and executes it if it is valid and outputs the effect.
	/// </summary>
	/// <returns>
	/// Whether the deploy order successfully executed or not.
	/// </returns>
	bool execute() override final;
	/// <summary>
	/// Sends the deploy order description to the output stream.
	/// </summary>
	friend ostream& operator<<(ostream& out, const DeployOrder& o);
	/// <summary>
	/// Assigns a copy of the deploy order description and effect to another deploy order variable.
	/// </summary>
	DeployOrder& operator=(const DeployOrder& o);
private:
	// Data members
	map::Territory* _targetTerritory{nullptr};
	int _numberOfArmies{0};
};

/// <summary>
/// Negotiate Order Class
/// </summary>
/// <remarks>
/// Can be validated and when executed, will prevent attacks between two players for one turn.
/// </remarks>
class NegotiateOrder : public Order {
public:
	// Constructors
	NegotiateOrder();
	NegotiateOrder(Player& issuingPlayer, Player& secondPlayer);
	NegotiateOrder(const NegotiateOrder& orderToCopy);
	// Destructor
	~NegotiateOrder();
	// Service methods
	/// <summary>
	/// Validates if the negotiate order is valid.
	/// </summary>
	/// <returns>
	/// Whether the negotiate order is valid or not.
	/// </returns>
	bool validate() override final;
	/// <summary>
	/// Validates the negotiate order, and executes it if it is valid and outputs the effect.
	/// </summary>
	/// <returns>
	/// Whether the negotiate order successfully executed or not.
	/// </returns>
	bool execute() override final;
	/// <summary>
	/// Sends the negotiate order description to the output stream.
	/// </summary>
	friend ostream& operator<<(ostream& out, const NegotiateOrder& o);
	/// <summary>
	/// Assigns a copy of the negotiate order description and effect to another negotiate order variable.
	/// </summary>
	NegotiateOrder& operator=(const NegotiateOrder& o);
private:
	// Data members
	Player* _secondPlayer{nullptr};
};


/// <summary>
/// This class contains a list of Order derived classes.
/// </summary>
/// <remarks>
/// You can add, move, and remove orders from the list.
/// </remarks>
class OrdersList {
public:
	// Constructors
	OrdersList();
	OrdersList(const OrdersList& ordersListToCopy);
	// Destructor
	~OrdersList();
	// Service methods
	/// <summary>
	/// Adds a given order to the orders list.
	/// </summary>
	/// <remarks>
	/// Any derived order can be added, even invalid ones.
	/// </remarks>
	/// <param name="orderToAdd">The order object being added to the list.</param>
	void add(Order* orderToAdd);
	/// <summary>
	/// Moves a given order in the list.
	/// </summary>
	/// <param name="oldIndex">The current index of the order being moved.</param>
	/// <param name="newIndex">The index of where the order should be moved.</param>
	void move(int oldIndex, int newIndex);
	/// <summary>
	/// Removes a given order from the list.
	/// </summary>
	/// <remarks>
	/// Remove is used instead of delete because delete is a reserved keyword.
	/// </remarks>
	/// <param name="index">The current index of order to be removed.</param>
	void remove(int index);
	bool empty();

	string toString();
	/// <summary>
	/// Sends a list of the orders currently in the list to the output stream.
	/// </summary>
	/// <param name="ordersListToStream">The orders list to send to the output stream.</param>
	friend ostream& operator<<(ostream& out, const OrdersList& ordersListToStream);
	/// <summary>
	/// Assigns a copy of the orders list to another orders list variable.
	/// </summary>
	/// <param name="ordersListToAssign">The orders list to copy over.</param>
	OrdersList& operator=(OrdersList& ordersListToAssign);
	
private:
	// Data members
	list<Order*> _orders;
};
