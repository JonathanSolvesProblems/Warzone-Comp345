#include "Map.h"


namespace map {

Map::Map() {

    territories = new std::vector<Territory*>();
    continents = new std::vector<Continent*>();
  }

  /*
   * Note: The individual territories and continents will not be copied.
   */
  Map::Map(const Map& other) {
   territories = new std::vector<Territory*>();
   territories->assign(other.territories->begin(), other.territories->end());

   continents = new std::vector<Continent *>();
   continents->assign(other.continents->begin(), other.continents->end());
  }

  Map::~Map() {
    delete territories;
    delete continents; 
  }

  void Map::addContinent(Continent* continent) {
    if (getContinent(continent->getID())) return; // TODO Change to throw exception

    this->continents->push_back(continent);
  }

  void Map::addTerritory(Territory* territory) {
    if (getTerritory(territory->getID())) return; // TODO Change to throw exception

    this->territories->push_back(territory);
  }

  void Map::connectTerritories(Territory *territory_a, Territory *territory_b)
  {
    if (territory_a && territory_b)
    {
      territory_a->addNeighbour(territory_b);
    }
  }

  void Map::connectTerritories(int id_a, int id_b) {
    Territory *territory_a = getTerritory(id_a);
    Territory *territory_b = getTerritory(id_b);

    connectTerritories(territory_a, territory_b);
  }

  Territory* Map::getTerritory(int id) const {
    for (Territory* territory : *territories) {
      if (territory->getID() == id) {
        return territory;
      }
    }
    return nullptr;
  }

  Territory* Map::getTerritory(std::string name) const {
    for (Territory *territory : *territories)
    {
      if (territory->getName() == name)
      {
        return territory;
      }
    }
    return nullptr;
  }

  Continent* Map::getContinent(int id) const
  {
    for (Continent* continent : *continents)
    {
      if (continent->getID() == id)
      {
        return continent;
      }
    }
    return nullptr;
  }

  Continent* Map::getContinent(std::string name) const
  {
    for (Continent *continent : *continents)
    {
      if (continent->getName() == name)
      {
        return continent;
      }
    }
    return nullptr;
  }

  bool Map::validate() {
    return false;
  }

  std::string Map::toString() const {
    return "(Map)";
  }

  std::ostream &
  operator<<(std::ostream &os, const Map &map)
  {
    os << map.toString();
    return os;
  }

}