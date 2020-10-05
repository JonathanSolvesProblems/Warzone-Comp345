#include <iostream>

#include "map/Map.h"


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

void test_territory () {
	// Testing constructor
	map::Continent continent = map::Continent(0, "test continent", 4);
	map::Territory territory = map::Territory(0, "test territory #1", continent);
	map::Territory territory_b = map::Territory(1, "test territory #2", continent);

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
}

int main() {
	std::cout << "Testing Continent Class" << std::endl
						<< std::endl;
	test_continent();

	std::cout << "Testing Territory Class" << std::endl
						<< std::endl;
	test_territory();
	
	return 0;
}