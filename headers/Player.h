#pragma once

class Player;

#include "Map.h"
#include "Card.h"
#include "Orders.h"
#include "PlayerStrategies.h"

#include "GameObservers.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::ostream;
using std::vector;
using std::unordered_set;
using std::list;

class PlayerStrategy;

class Player : public Observable {
public:

	// an order list
	OrdersList* listOfOrders{nullptr};
	// a vector of cards to hold the players hand
	Hand *hand{nullptr};
	// stores the ID's of the defending territories
	vector<map::Territory*> owned_territories;

	//stores the player name
	std::string playerName;

	// Gets the number of armies the player has
	int getArmees() const; 

	// Sets the number of armies the player has
	void setArmees(int number);

	//stores a player's ID
	int playerID;

	
	/// <summary>
	/// Player constructor
	/// </summary>
	/// <param name="name">Name of player</param>
	/// <param name="pID">Player's identifier</param>
	Player(std::string name, int pID);

	// initializes new strategy
	Player(PlayerStrategy * initStrategy);

	// change strategy
	void setStrategy(PlayerStrategy * newStrategy);

	/// <summary>
	/// Copy Constructor
	/// </summary>
	/// <param name="orderToCopy">Copy order</param>
	Player(const Player& toCopy);

	/// <summary>
	/// Delete Player's only pointer field
	/// </summary>
	~Player();

	/// <summary>
	/// Outstream prints the player's name
	/// </summary>
	/// <param name="out"></param>
	/// <param name="playerToStream"></param>
	/// <returns></returns>
	friend ostream& operator<<(ostream& out, const Player& o);

	/// <summary>
	/// Assigns all fields of player to a new player
	/// </summary>
	/// <param name="playerToAssign"></param>
	/// <returns></returns>
	Player operator=(const Player& o);

	/// <summary>
	/// Draws a card from the deck and inserts it into the player's hand.
	/// </summary>
	/// <param name="hand">Hand object</param>
	/// <param name="deck">Deck object</param>
	void draw(Deck& deck);

	/// <summary>
	/// Returns the a list of pointers to Territories that the player will defend.
	/// For now, these are all the territories owned by the player.
	/// </summary>
	/// <param name="test"></param>
	const vector<map::Territory*> toDefend(); // TODO: Remove method here, once strategy definitions are implemented

	/// <summary>
	/// Returns the territories to attack (All neighbouring territories owned by other players).
	/// </summary>
	const vector<map::Territory*> toAttack(); // TODO: Remove method here, once strategy definitions are implemented

	/// <summary>
	/// Adds order to the player's list of orders
	/// </summary>
	/// <returns>Order to issue, nullptr if player is done</returns>
	Order* issueOrder(); // TODO: Remove method here, once strategy definitions are implemented

	// Returns the next order to be executed following priority:
	// 			Deploy, Airlift, Blockade, Others
	Order* nextOrder();

    Order* issueAdvanceOrder();
    Order* issueAirliftOrder();
    Order* issueBombOrder();
    Order* issueBlockadeOrder();
	Order* issueDeployOrder();
    Order* issueNegotiateOrder();

	// Adds a territory to myTerritories (Does not set the Territory's owner)
	void addTerritory(map::Territory*);

	// Removes a territory from myTerritories (Does not unset the Territory's owner)
	void removeTerritory(map::Territory*);

	// Returns true if the player owns the given territory
	bool isOwner(map::Territory*);



private:
	// Tracks the number of reinforcements the player has available
	int armees;
	int _armees_to_deploy_this_round;
	vector<map::Territory *> _targetsThisRound{nullptr};
	vector<map::Territory*> _defencesThisRound{nullptr};
	PlayerStrategy * _strategy;
};

