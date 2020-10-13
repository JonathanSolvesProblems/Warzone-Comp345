#include <iostream>
#include "Player.h"
#include "MapLoader.h"
using map::Territory;

int main() {

	vector<int> ID;
	map::Map test;
	MapLoader map;
	map.loadFile("exampleMaps/solar.map", test);

	// remove 2 at the end
	cout << "Continents  " << map.continents.size() << " \n";
	for (int s = 0; s < map.continents.size(); s++)
	{
		cout << *test.getContinent(s + 1) << endl;
	}

	cout << "Countries and Borders" << map.borders.size() << "\n";
	for (int s = 0; s < map.borders.size(); s++)
	{
		// stores the territory ID in a vector on its own
		ID.push_back(test.getTerritory(s + 1)->getID());
		cout << "Country: " << *test.getTerritory(s + 1) << " Has This many " << test.getTerritory(s + 1)->getNeighbourCount() << " Neighbouring Countries And Belongs to " << test.getTerritory(s + 1)->getContinent() << endl;
	}

	//TODO next iteration
	// have to run a forloop through all the neighbors to display if it is a neighbour 
	// try to figure out how to send a vector containign the IDs of the neighborung territoryeis 
	// End of making the map/loading the map 
	
	//Testing player class below...
	Player playa("brenden", 2, ID);
	cout << "Greetings Player: " << playa.playerName << "\n";
	cout << "Your Player ID is: " << playa.playerID << "\n";

	// displays all available territory IDs from the player class
	cout << "All available territories IDs" << "\n";
	for (int s = 0; s < playa.allTerritories.size(); s++)
	{
		cout << playa.allTerritories[s] << "\n";
	}

	cout << "\n";
	cout << "\n";
	cout << "Player " << playa << " Has acquired Territory 1" << "\n";
	cout << "Player " << playa << " Has acquired Territory 52" << "\n";

	// Adding Territory ID to the player's territory list
	playa.myTerritories.push_back(test.getTerritory(1)->getID());
	playa.myTerritories.push_back(test.getTerritory(52)->getID());

	// Displaying territory to defend (need to pass a map object to determine what the teritory actually is) 
	playa.toDefend(test);
	cout << "\n";
	cout << "\n";

	// Displays all other territories that could be attacked (have to implement the neighbours method to actually check for the proper neighbours
	playa.toAttack(test);
	cout << "\n";
	cout << "\n";

	//Card section
	cout << "Implementing Player with Cards below:\n\n";
	Hand hand;
	Deck deck;

	playa.playerHand(hand, deck);

	//Order List
	cout << "Implementing Player with Orders below:\n\n";
	AdvanceOrder o1;
	AirliftOrder o2;
	NegotiateOrder o3;

	AdvanceOrder* o1p = &o1;
	AirliftOrder* o2p = &o2;
	NegotiateOrder* o3p = &o3;

	//IssueOrder accepting  order pointers to add to the list
	playa.issueOrder(o1p);
	playa.issueOrder(o2p);
	playa.issueOrder(o3p);

	//Testing overload assignment and copy constructor
	Player playaCopy(playa);
	Player playaAssigned = playaCopy;

	//Testing outstream overload for player
	cout << "Player's current list of orders: \n" << *(playa.listOfOrders);
	cout << "\n Checking player has copied, should be same as playa's name brenden: " << playaCopy  << "\n" ;
	cout << "\n Checking assignment operator overload, should be same as playa's name brenden: " << playaAssigned << "\n";

	return 0;
}