#pragma once
// #define DEBUG // Remove this to disabled debugging

namespace map {
  class Map;
  class Territory;
  class Continent;
}

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <utility>

#include "Player.h"

namespace map
{

/*
Represents a Warzone Map as a connected graph of territories.

The class has been designed with simplicity in mind, not performance. The underlying data
structure is certainly not the most efficient for very large maps, but for reasonably sized
maps the performance is more than acceptable.
*/
class Map
{

private:

  /*
   * Keeps track of each territory that has been added to the map. Each Territory maintains a
   * a vector of pointers to its neighbour.
   */
  std::vector<Territory*>* territories{nullptr};

  /*
   * Stores the continents added to the map.
   */
  std::vector<Continent*>* continents{nullptr};

  /*
   * Performs a depth-first search of the graph and verifies that each node is visited in the search.
   * Returns true iff the the graph is connected.
   */
  bool isConnectedGraph();

  /*
   * Used to recursively perform a depth-first traversal of the graph represented
   * by the territories.
   * 
   * Args:
   *    Territory* territory: The territory to start the traversal from.
   *    std::unordered_map<int, bool>& territory_ids_visited_map: Used to mark which
   *        territories have been visited. Must be initialized with a mapping from
   *        each territory id that will be visited to a default value of false.
   */
  void traverseGraph(Territory *territory, std::unordered_map<int, bool> &territory_ids_visited_map);

  /*
   * Performs a depth-first search of each continent, with the constraint that it may not travel
   * to nodes belonging to a continent. If every node in the continent is visited, then the
   * continent is connected.
   * 
   * Returns true iff every Continent in continents is 
   */
  bool everyContinentIsConnectedSubgraph();

  /*
   * Verifies that each territory is referenced by a single continent.
   */
  bool eachTerritoryHasSingleContinent();

public:

  /*
   * Default Constructor. Initializes the vectors for continents and territories.
   */
  Map();

  /*
   * Copy Constructor. Constructs a new map from an existing one.
   * NOTE: Territories and Continents are shallow copied.
   */
  Map(const Map& other);

  /*
   * Destroys continents and territories added to the map.
   */
  ~Map();

  /*
   * Adds a Continent to the Map.
   * 
   * Args:
   *    Continent* continent: A pointer to the Continent that will be added.
   */
  void addContinent(Continent* continent);
  
  /*
   * Adds a Territory to the Map.
   * 
   * Args:
   *    Territory* territory: A pointer to the Territory that will be added.
   */
  void addTerritory(Territory*);

  /*
   * Connects two territories. The territories should both already have been
   * added to the map. The order is not important as the relationship is
   * bi-directional.
   * 
   * Args:
   *    Territory* territory_a: A pointer to one of the territories to connect.
   *    Territory* territory_b: A pointer to the other territory.
   */
  void connectTerritories(Territory* territory_a, Territory* territory_b);

  /*
   * Connects two territories. The territories should both already have been
   * added to the map. The order is not important as the relationship is
   * bi-directional.
   * 
   * Args:
   *    int territory_a: The ID of one of the territories to connect.
   *    int territory_b: The ID of the other territory.
   */
  void connectTerritories(int territory_id_a, int territory_id_b);

  /*
   * Find a territory by id.
   * 
   * Args:
   *    int id: The id to search for.
   * 
   * Returns:
   *    Territory*: A pointer to the territory with the corresponding id,
   *        or nullptr if not found.
   */
  Territory* getTerritory(int id) const;

  /*
   * Returns a readonly iterator of the territories.
   */
  const std::vector<Territory *> getTerritories();

  /*
   * Find a territory by name.
   * 
   * Args:
   *    std::string name: The name to search for.
   * 
   * Returns:
   *    Territory*: A pointer to the first territory found with the corresponding
   *        name, or nullptr if not found.
   */
  Territory* getTerritory(std::string) const;

  /*
   * Find a continent by id.
   * 
   * Args:
   *    int id: The id to search for.
   * 
   * Returns:
   *    Continent*: A pointer to the continent with the corresponding id,
   *        or nullptr if not found.
   */
  Continent* getContinent(int) const;

  /*
   * Find a continent by name.
   * 
   * Args:
   *    std::string name: The name to search for.
   * 
   * Returns:
   *    Continent*: A pointer to the first continent found with the corresponding
   *        name, or nullptr if not found.
   */
  Continent* getContinent(std::string) const;

  /*
   * Returns true iff the Map meets the following criteria:
   *    1. The map represents a connected graph (every territory is reachable from any other)
   *    2. Each continent represents a connected sub-graph (every territory in the continent
   *          can be reached without "leaving" the continent).
   *    3. Each territory belongs to exactly one continent.
   */
  bool validate();

  /*
   * Returns a std::string representation of the Map.
   */
  std::string toString() const;

  /*
   * Inserts the Map's string representation to the std::ostream
   */
  friend std::ostream &operator<<(std::ostream& os, const Map& map);
};


/*
 * Stores all information for a territory and represents a node of the Map graph.
 * 
 * Edges between nodes are represented as a vector of pointers to neighbouring
 * Territories. This design was chosen for its simplicity, as we deemed performance
 * not to be a priority due to the limited size of the maps.
 */
class Territory
{
private:
  // The id of the territory. Used mainly by the MapLoader class.
  int *id{nullptr};
  // A name used to identify the territory to users.
  std::string *name{nullptr};
  // A pointer to the Continent this territory belongs to.
  Continent *continent{nullptr};
  
  // A pointer the the player who owns this territory.
  Player *owner_player{nullptr};

  int armees;

  /*
   * Stores pointers to the neighbouring territories.
   * 
   * When neighbours contains a pointer to another territory, it means
   * that the two territories are adjacent in the graph represented by
   * the map.
   */
  std::vector<Territory *> *neighbours{nullptr};

  /*
   * Finds a neighbour (by pointer) and removes it from the vector.
   * 
   * Args:
   *    Territory* territory: A pointer to the neighbour which should
   *        be detached.
   */
  Territory *_findAndEraseNeighbour(Territory * territory);

public:
  /*
   * Constructs a new Territory
   * 
   * Args:
   *    int id
   *    std::string name
   *    Continent& continent: A reference to the continent this territory will
   *        belong to.
   */
  Territory(int id, std::string name, Continent& continent);

  /*
   * Copy constructor.
   * NOTE: Neighbours are shallow copied (pointers only, not objects)
   */
  Territory(const Territory& territory);

  /*
   * Ensures that deleting this territory will not leave dangling pointers
   * in the continent or neighbours which refer to it.
   */
  ~Territory();

  /*
   * Uses the copy constructor for assignment
   */
  Territory operator=(const Territory &);

  /*
   * Returns a std::string representation of the Territory in the following
   * format:
   *    (Territory #<ID>) <name>
   */
  std::string toString() const;

  /*
   * Returns the integer ID of the territory.
   */
  int getID() const;

  /*
   * Returns number of armees on the territory.
   */
  int getArmees() const;

  /*
   * Returns a pointer to this territories Continent.
   */
  Continent* getContinent() const;

  /*
   * Returns the name of the Territory.
   */
  std::string getName() const;

  /*
   * Returns the pointer to the player which currently owns the territory.
   * NOTE: Returns nullptr if the territory is not owned by any player.
   */
  Player* getOwner() const;

  /*
   * Sets the owning player, adds this territory the player's list of territories
   * and removes it from the previous player's list.
   */
  void setOwner(Player* player);

  /* Sets the number of armees on the territory. */
  void setArmees(int number);

  /*
   * Returns the number of neighbours this territory has.
   */
  int getNeighbourCount() const;

  /*
   * Finds a neighbour by ID and returns a pointer to it,
   * or nulltr if not found.
   */
  Territory *getNeighbour(int id) const;

  /*
   * Finds the first neighbour matching name and returns a pointer to it,
   * or nulltr if not found.
   */
  Territory *getNeighbour(std::string name) const;

  /*
   * Returns a readonly iterator of the Territory's neighbours.
   */
  const std::vector<Territory *> getNeighbours() const;

  /*
   * Returns true iff the Territory has a neighbour matching the given id.
   */
  bool hasNeighbour(int id) const;

  /*
   * Returns true iff this territory is a neighbour of the Territory pointed
   * to by the given pointer.
   */
  bool hasNeighbour(Territory *territory) const;

  /*
   * Adds the pointer to this Territory's vector of neighbours, and ensures that the given
   * territory has a reference to this.
   * 
   * If the territories are already neighbours, nothing is done.
   */
  Territory *addNeighbour(Territory *territory);

  /*
   * Sets this Territory's continent to the given pointer.
   * 
   * NOTE: Territories should be added to a continent using
   *     Continent.addTerritory
   */
  void setContinent(Continent* continent);

  /*
   * Removes the adjacency between this Territory and the one
   * pointed to by the given pointer.
   */
  void deleteNeighbour(Territory * territory);

  /*
   * Removes the adjacency between this Territory and the one
   * with the given id.
   */
  void deleteNeighbour(int id);

  /*
   * Removes the adjacency between this Territory and the first one
   * with the given name.
   */
  void deleteNeighbour(std::string);

  /*
   * Inserts the territory's string representation to the std::ostream
   */
  friend std::ostream &operator<<(std::ostream &, const Territory &);
};

/*
 * Stores the id, name and bonus of a Continent.
 * 
 * Maintains pointers to each territory in the continent.
 */
class Continent
{

private:
  // The ID of the continent (Used primarly by MapLoader)
  int *id{nullptr};
  
  // The name of the continent, used to display the Continent to the user.
  std::string *name{nullptr};

  // A bonus gained by the player when they occupy every territory in the continent.
  int *bonus{nullptr};

  // Maintains pointers to every territory in the continent.
  std::vector<Territory*>* territories{nullptr};

  /*
   * Starting at the given territory, traverses the continent without leaving it.
   * 
   * Args:
   *    Territory* territory: A pointer to the territory from which to start the traversal.
   *    std::unordered_map<int, bool>: Keeps track of which Territories have been visited.
   *        Note: must be initialized with the id of each territory that may be visited, and
   *          a default value of false.
   */
  void traverseContinentWithoutLeaving(Territory*, std::unordered_map<int, bool>&);

public :

  /*
   * Creates a new Continent.
   * 
   * Args:
   *    int id: The id of the continent.
   *    std::string name: the name of the continent.
   *    int bonus: The bonus of the continent.
   */
  Continent(int id, std::string name, int bonus);

  // Copy Constructor. Shallow copies the territories.
  Continent(const Continent &);

  /*
   * Ensures that there are no dangling pointers on the territories when
   * the continent is destroyed.
   */
  ~Continent();

  // Uses the copy constructor for assignment
  Continent operator=(const Continent &);

  // Inserts the continents string representation into the std::ostream.
  friend std::ostream &operator<<(std::ostream &, const Continent &);

  /*
   * Returns a std::string representation of the Continent as:
   *    (Continent #<ID>) <NAME> +<BONUS>
   */
  std::string toString() const;

  /* Returns the integer ID of the continent */
  int getID() const;

  /* Returns the name of the continent */
  std::string getName() const;

  /*
   * Returns the army bonus earned when a player occupies the whole continent.
   */
  int getBonus() const;

  /*
   * Returns a readonly iterator of the continents territories.
   */
  const std::vector<Territory*> getTerritories();

  /*
   * Checks whether the given territory is in
   * this continents vector of territories.
   */
  bool hasTerritory(Territory*);

  /*
   * Adds a territory to the continent and sets the
   * territory's continent to this.
   */
  void addTerritory(Territory*);

  /*
   * Removes the given territory from the continent,
   * and sets the territory's continent to nullptr.
   */
  void removeTerritory(Territory*);

  /*
   * Performs a depth-first traversal of the continent
   * without "leaving" the continent.
   * 
   * Return true iff any territory in the continent is reachable
   * from any other territory without leaving the continent.
   */
  bool isConnectedSubGraph();
};
}