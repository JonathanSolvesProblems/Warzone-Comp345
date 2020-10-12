#pragma once

#include <iostream>
#include <list>
using namespace std;

class Order {
public:
	// Constructors
	Order();
	Order(string description, string effect);
	Order(const Order& orderToCopy);
	// Destructor
	~Order();
	// Service methods
	virtual bool validate() = 0;
	virtual bool execute() = 0;
	friend ostream& operator<<(ostream& out, const Order& o);
	virtual Order& operator=(const Order& o);
protected:
	// Data members
	string* _description;
	string* _effect;
};

class AdvanceOrder : public Order {
public:
	// Constructors
	AdvanceOrder();
	AdvanceOrder(const AdvanceOrder& orderToCopy);
	// Destructor
	~AdvanceOrder();
	// Service methods
	bool validate() override final;
	bool execute() override final;
	friend ostream& operator<<(ostream& out, const AdvanceOrder& o);
	AdvanceOrder& operator=(const AdvanceOrder& o);
};

class AirliftOrder : public Order {
public:
	// Constructors
	AirliftOrder();
	AirliftOrder(const AirliftOrder& orderToCopy);
	// Destructor
	~AirliftOrder();
	// Service methods
	bool validate() override final;
	bool execute() override final;
	friend ostream& operator<<(ostream& out, const AirliftOrder& o);
	AirliftOrder& operator=(const AirliftOrder& o);
};

class BlockadeOrder : public Order {
public:
	// Constructors
	BlockadeOrder();
	BlockadeOrder(const BlockadeOrder& orderToCopy);
	// Destructor
	~BlockadeOrder();
	// Service methods
	bool validate() override final;
	bool execute() override final;
	friend ostream& operator<<(ostream& out, const BlockadeOrder& o);
	BlockadeOrder& operator=(const BlockadeOrder& o);
};

class BombOrder : public Order {
public:
	// Constructors
	BombOrder();
	BombOrder(const BombOrder& orderToCopy);
	// Destructor
	~BombOrder();
	// Service methods
	bool validate() override final;
	bool execute() override final;
	friend ostream& operator<<(ostream& out, const BombOrder& o);
	BombOrder& operator=(const BombOrder& o);
};

class DeployOrder : public Order {
public:
	// Constructors
	DeployOrder();
	DeployOrder(const DeployOrder& orderToCopy);
	// Destructor
	~DeployOrder();
	// Service methods
	bool validate() override final;
	bool execute() override final;
	friend ostream& operator<<(ostream& out, const DeployOrder& o);
	DeployOrder& operator=(const DeployOrder& o);
};

class NegotiateOrder : public Order {
public:
	// Constructors
	NegotiateOrder();
	NegotiateOrder(const NegotiateOrder& orderToCopy);
	// Destructor
	~NegotiateOrder();
	// Service methods
	bool validate() override final;
	bool execute() override final;
	friend ostream& operator<<(ostream& out, const NegotiateOrder& o);
	NegotiateOrder& operator=(const NegotiateOrder& o);
};

class OrdersList {

public:
	// Constructors
	OrdersList();
	OrdersList(const OrdersList& ordersListToCopy);
	// Destructor
	~OrdersList();
	// Service methods
	void add(Order* orderToAdd);
	void move(int oldIndex, int newIndex);
	void remove(int index); // remove is used instead of delete because delete is a reserved keyword
	friend ostream& operator<<(ostream& out, const OrdersList& ordersListToStream);
	OrdersList& operator=(OrdersList& ordersListToAssign);

private:
	// Data members
	list<Order*> _orders;
};
