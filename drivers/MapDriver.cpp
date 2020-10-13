#include <iostream>

#include "Map.h"

/* Will test the basic behaviours of the Continent class */
void test_continent () {
	// Testing Constructor
	const map::Continent continent = map::Continent(0, "test continent", 4);
	std::cout << continent << std::endl;

	// Test getters
	std::cout << continent.getName() << std::endl;
	std::cout << continent.getID() << std::endl;
	std::cout << continent.getBonus() << std::endl;

	// Test copy assignment
	const map::Continent other_continent = map::Continent(continent);
	std::cout << other_continent.getName() << std::endl;
}

/* Will test the behaviours of the Territory class */
void test_territory () {
	// Testing constructor
	map::Continent continent = map::Continent(0, "test continent", 4);
	map::Territory territory = map::Territory(0, "test territory #1", continent);
	map::Territory territory_b = map::Territory(1, "test territory #2", continent);

	territory.setArmees(4);

	std::cout << "First " << territory << std::endl;
	std::cout << "Second " << territory_b << std::endl;

	std::cout << "Neightbours " << territory.getNeighbourCount() << std::endl;
	territory.addNeighbour(&territory_b);

	std::cout << "Neighbours after adding " << territory.getNeighbourCount() << std::endl;
	std::cout << "B's neighbours " << territory_b.getNeighbourCount() << std::endl;

	map::Territory* found_territory = territory.getNeighbour("test territory #2");
	std::cout << "Equal? " << (found_territory == &territory_b) << std::endl;

	territory_b.deleteNeighbour(0);
	std::cout << "Deleted neighbour with id 0" << std::endl;
	std::cout << "Neighbours after deleting " << territory_b.getNeighbourCount() << std::endl;
	std::cout << "B's neighbours after deleting " << territory.getNeighbourCount() << std::endl;

	std::cout << "Creating territory on the heap to test dangling pointers:" << std::endl;
	map::Territory* territory_c = new map::Territory(2, "dynamic territory", continent);

	territory.addNeighbour(territory_c);
	std::cout << "Neighbours after adding dynamic: " << territory_c->getNeighbourCount() << std::endl;
	std::cout << "Deleting!" << std::endl;

	delete territory_c;
	territory_c = nullptr;

	std::cout << "Neighbours after deleting: " << territory.getNeighbourCount() << std::endl;

	std::cout << "Testing Player owner" << std::endl;
	Player owner = Player("Player 1", 1);
	Player new_owner = Player("Player 2", 2);

	territory.setOwner(&owner);

	std::cout << "Territory A is owned by " << *territory.getOwner() << std::endl;
	std::cout << "Player 1 has " + std::to_string(owner.owned_territories.size()) + " territories" << std::endl;
	std::cout << "Player 2 has " + std::to_string(new_owner.owned_territories.size()) + " territories" << std::endl;

	std::cout << "Reassigning owner..." << std::endl;

	territory.setOwner(&new_owner);

	std::cout << "Territory A is owned by " << *territory.getOwner() << std::endl;
	std::cout << "Player 1 has " + std::to_string(owner.owned_territories.size()) + " territories" << std::endl;
	std::cout << "Player 2 has " + std::to_string(new_owner.owned_territories.size()) + " territories" << std::endl;
}

/* Will test the behaviours of the Continent class */
void test_map () {
	map::Map* map = new map::Map();
	std::cout << *map << std::endl;	

	map->addContinent(new map::Continent(0, "test continent", 3));
	std::cout << "Added Test Continent" << std::endl;
	map->addTerritory(new map::Territory(0, "territory A", *(map->getContinent("test continent"))));
	std::cout << "Added Test Territory A" << std::endl;

	map->addTerritory(new map::Territory(1, "territory B", *(map->getContinent("test continent"))));
	std::cout << "Added Test Territory B" << std::endl;

	map->addTerritory(new map::Territory(2, "territory C", *(map->getContinent("test continent"))));
	std::cout << "Added Test Territory C" << std::endl;

	map->connectTerritories(map->getTerritory("territory A"), map->getTerritory("territory B"));
	std::cout << "Connected A to B" << std::endl;

	map->connectTerritories(1, 2);
	std::cout << "Connected B to C" << std::endl;

	map->connectTerritories(0, 2);
	std::cout << "Connected C to A (A to C)" << std::endl;

	std::cout << "A <-> B " << map->getTerritory("territory A")->hasNeighbour(1) << std::endl;
	std::cout << "B <-> C " << map->getTerritory("territory B")->hasNeighbour(2) << std::endl;
	std::cout << "A <-> C " << map->getTerritory("territory C")->hasNeighbour(0) << std::endl;

	std::cout << map->validate() << std::endl;
}

int main() {
	std::cout << "Testing Continent Class" << std::endl
						<< std::endl;
	test_continent();

	std::cout << "Testing Territory Class" << std::endl
						<< std::endl;
	test_territory();

	std::cout << "Testing Map Class" << std::endl
						<< std::endl;
	test_map();

	return 0;
}