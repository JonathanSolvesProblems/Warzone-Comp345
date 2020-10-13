#include <iostream>
#include "Player.h"
#include "MapLoader.h"
using map::Territory;

int main() {

	map::Map map = map::Map();
	MapLoader loader;
	loader.loadFile("exampleMaps/solar.map", map);
	
	//Testing player class below...
	Player* player = new Player("brenden", 2);
	cout << "Greetings Player: " << player->playerName << "\n";
	cout << "Your Player ID is: " << player->playerID << "\n";

	cout << "Acquiring Territory: " << *(map.getTerritory(1)) << std::endl;
	map.getTerritory(1)->setOwner(player);
	cout << "Acquiring Territory: " << *(map.getTerritory(3)) << std::endl;
	map.getTerritory(1)->setOwner(player);

	// Displaying territory to defend (need to pass a map object to determine what the teritory actually is) 
	cout << "Territories to defend:" << endl;
	for (Territory* territory : player->toDefend()) {
		cout << '\t' << *territory << std::endl;
	}

	// Displays all other territories that could be attacked (have to implement the neighbours method to actually check for the proper neighbours
	cout << "Territories to attack:" << endl;
	for (Territory *territory : player->toAttack())
	{
		cout << '\t' << *territory << std::endl;
	}

	// Card section
	cout << "Drawing 3 cards to hand:" << std::endl;
	Deck deck = Deck();
	player->draw(deck);
	player->draw(deck);
	player->draw(deck);

	cout << "Cards in hand:" << std::endl;
	player->hand->show();

	//Order List
	cout << "Testing Orders:" << std::endl;

	//IssueOrder accepting  order pointers to add to the list
	player->issueOrder(new AdvanceOrder());
	player->issueOrder(new AirliftOrder());
	player->issueOrder(new NegotiateOrder());

	//Testing outstream overload for player
	cout << "Player's current list of orders:" << std::endl << *(player->listOfOrders) << std::endl;

	//Testing overload assignment and copy constructor
	cout << "Testing Copy and Assignment: " << std::endl;
	
	Player playaCopy(*player);
	Player playaAssigned = playaCopy;

	cout << "Checking player has copied, should be same as player's name brenden: " << playaCopy << std::endl;
	cout << "Checking assignment operator overload, should be same as player's name brenden: " << playaAssigned << std::endl;

	delete player;
	cout << "DESTROYED PLAYER!" << endl;

	return 0;
}