#include <iostream>
#include "Player.h"
#include "MapLoader.h"
using map::Territory;

using std::cout;
using std::endl;

int main() {

	map::Map map = map::Map();
	MapLoader loader;
	loader.loadFile("solar.map", map);
	
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
	player->draw(*Deck::instance());
	player->draw(*Deck::instance());
	player->draw(*Deck::instance());

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

	delete player;

	return 0;
}