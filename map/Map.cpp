#include "Map.h"

namespace map
{

  /*
   * MAP CLASS DEFINITIONS
   */

  Map::Map()
  {
    territories = new std::vector<Territory *>();
    continents = new std::vector<Continent *>();
  }

  /*
   * Note: The individual territories and continents will not be copied.
   */
  Map::Map(const Map &other)
  {
    territories = new std::vector<Territory *>();
    territories->assign(other.territories->begin(), other.territories->end());

    continents = new std::vector<Continent *>();
    continents->assign(other.continents->begin(), other.continents->end());
  }

  Map::~Map()
  {
    delete territories;
    delete continents;
  }

  void Map::addContinent(Continent *continent)
  {
    if (getContinent(continent->getID()))
      return; // TODO Change to throw exception

    this->continents->push_back(continent);
  }

  void Map::addTerritory(Territory *territory)
  {
    if (getTerritory(territory->getID()))
      return; // TODO Change to throw exception

    this->territories->push_back(territory);
  }

  void Map::connectTerritories(Territory *territory_a, Territory *territory_b)
  {
    if (territory_a && territory_b)
    {
      territory_a->addNeighbour(territory_b);
    }
  }

  void Map::connectTerritories(int id_a, int id_b)
  {
    Territory *territory_a = getTerritory(id_a);
    Territory *territory_b = getTerritory(id_b);

    connectTerritories(territory_a, territory_b);
  }

  Territory *Map::getTerritory(int id) const
  {
    for (Territory *territory : *territories)
    {
      if (territory->getID() == id)
      {
        return territory;
      }
    }
    return nullptr;
  }

  Territory *Map::getTerritory(std::string name) const
  {
    for (Territory *territory : *territories)
    {
      if (territory->getName() == name)
      {
        return territory;
      }
    }
    return nullptr;
  }

  Continent *Map::getContinent(int id) const
  {
    for (Continent *continent : *continents)
    {
      if (continent->getID() == id)
      {
        return continent;
      }
    }
    return nullptr;
  }

  Continent *Map::getContinent(std::string name) const
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

  bool Map::validate()
  {
    return isConnectedGraph() && everyContinentIsConnectedSubgraph(); // && eachTerritoryHasSingleContinent();
  }

  bool Map::isConnectedGraph()
  {
    std::cout << "CHECKING IF CONNECTED GRAPH" << std::endl;
    std::unordered_map<int, bool> territory_ids_visited_map = std::unordered_map<int, bool>();
    territory_ids_visited_map.reserve(territories->size());

    std::cout << "INSERTING TERRITORIES" << std::endl;
    for (Territory *territory : *territories)
    {
      territory_ids_visited_map.insert(std::make_pair(territory->getID(), false));
    }
    std::cout << "INSERTED TERRITORIES" << std::endl;
    std::cout << "BEGIN " << **(territories->begin()) << std::endl;
    traverseGraph(*(territories->begin()), territory_ids_visited_map);

    std::cout << "TRAVERSED GRAPH" << std::endl;

    return std::all_of(
        territory_ids_visited_map.begin(),
        territory_ids_visited_map.end(),
        [](auto pair) { return pair.second; });
  }

  bool Map::everyContinentIsConnectedSubgraph()
  {
    return std::all_of(
        continents->begin(),
        continents->end(),
        [](Continent *continent) { return continent->isConnectedSubGraph(); });
  }

  bool Map::eachTerritoryHasSingleContinent()
  {
    return true;
  }

  void Map::traverseGraph(Territory *territory, std::unordered_map<int, bool> &territory_ids_visited_map)
  {
    territory_ids_visited_map[territory->getID()] = true;
    for (Territory *neighbour : territory->getNeighbours())
    {
      if (!territory_ids_visited_map[neighbour->getID()])
      {
        std::cout << "GOING TO " << *neighbour << " FROM" << *territory << std::endl;
        traverseGraph(neighbour, territory_ids_visited_map);
      }
    }
  }

  std::string Map::toString() const
  {
    return "(Map)";
  }

  std::ostream &
  operator<<(std::ostream &os, const Map &map)
  {
    os << map.toString();
    return os;
  }

  // END OF MAP CLASS DEFINITIONS

  /*
   * TERRITORY CLASS DEFINITIONS
   */
  Territory::Territory(int id, std::string name, Continent &continent)
  {
    this->id = new int(id);
    this->name = new std::string(name);
    this->continent = &continent;
    this->continent->addTerritory(this);
    this->neighbours = new std::vector<Territory *>();
  }

  Territory::~Territory()
  {
    delete id;
    delete name;

    // NOTE: The territories refered to by neighbours will not be deleted,
    //       which means that there will be dangling pointers in the neighbours
    //       vector of each neighbour (because the relationships go both ways).
    for (Territory *neighbour : *neighbours)
    {
      neighbour->deleteNeighbour(this);
    }
    delete neighbours;

    if (continent)
    {
      continent->removeTerritory(this);
    }
  }

  std::string Territory::toString() const
  {
    return "(Territory #" + std::to_string(*id) + ") " + *name;
  }

  int Territory::getID() const
  {
    return *id;
  }

  std::string Territory::getName() const
  {
    return std::string(*name);
  }

  Continent *Territory::getContinent() const
  {
    return continent;
  }

  int Territory::getNeighbourCount() const
  {
    return neighbours->size();
  }

  Territory *Territory::getNeighbour(int neighbour_id) const
  {
    for (Territory *neighbour : *neighbours)
    {
      if (neighbour->getID() == neighbour_id)
      {
        return neighbour;
      }
    }

    return nullptr;
  }

  Territory *Territory::getNeighbour(std::string neighbour_name) const
  {
    for (Territory *neighbour : *neighbours)
    {
      if (neighbour->getName() == neighbour_name)
      {
        return neighbour;
      }
    }

    return nullptr;
  }

  const std::vector<Territory *> Territory::getNeighbours() const
  {
    return *neighbours;
  }

  bool Territory::hasNeighbour(int neighbour_id) const
  {
    return getNeighbour(neighbour_id) != nullptr;
  }

  bool Territory::hasNeighbour(Territory *neighbour) const
  {
    if (neighbour != nullptr)
    {
      return getNeighbour(neighbour->getID()) == neighbour;
    }
    else
    {
      return false;
    }
  }

  Territory *Territory::addNeighbour(Territory *territory)
  {
    if (!hasNeighbour(territory))
    {
      neighbours->push_back(territory);
      territory->neighbours->push_back(this);
    }
    return territory;
  }

  void Territory::setContinent(Continent *continent)
  {
    this->continent = continent;
  }

  Territory *Territory::_findAndEraseNeighbour(Territory *territory)
  {
    for (auto it = neighbours->begin(); it != neighbours->end(); ++it)
    {
      if ((*it) == territory)
      {
        neighbours->erase(it);
        return (*it);
      }
    }
    return nullptr;
  }

  void Territory::deleteNeighbour(Territory *territory)
  {
    Territory *deletedTerritory = _findAndEraseNeighbour(territory);
    if (deletedTerritory)
    {
      deletedTerritory->_findAndEraseNeighbour(this);
    }
  }

  void Territory::deleteNeighbour(int id)
  {
    for (auto it = neighbours->begin(); it != neighbours->end(); ++it)
    {
      if ((*it)->getID() == id)
      {
        (*it)->_findAndEraseNeighbour(this);
        neighbours->erase(it);
        return;
      }
    }
  }

  void Territory::deleteNeighbour(std::string name)
  {
    for (auto it = neighbours->begin(); it != neighbours->end(); ++it)
    {
      if ((*it)->getName() == name)
      {
        neighbours->erase(it);
        return;
      }
    }
  }

  std::ostream &
  operator<<(std::ostream &os, const Territory &territory)
  {
    os << territory.toString();
    return os;
  }

  // END OF TERRITORY CLASS DEFINITIONS

  /*
   * CONTIENT CLASS DEFINITIONS
   */
  Continent::Continent(int id, std::string name, int bonus)
  {
    this->id = new int(id);
    this->name = new std::string(name);
    this->bonus = new int(bonus);
    this->territories = new std::vector<Territory *>();
  }

  Continent::Continent(const Continent &other)
  {
    id = new int(*other.id);
    bonus = new int(*other.bonus);
    name = new std::string(*other.name);

    territories = new std::vector<Territory *>();
    territories->assign(other.territories->begin(), other.territories->end());
  }

  Continent::~Continent()
  {
    delete id;
    delete bonus;
    delete name;

    for (Territory *territory : *territories)
    {
      if (territory)
      {
        territory->setContinent(nullptr);
      }
    }
    delete territories;
  }

  Continent Continent::operator=(const Continent &other)
  {
    return Continent(other);
  }

  std::string Continent::toString() const
  {
    return "(Continent #" + std::to_string(*id) + ") " + *name + " +" + std::to_string(*bonus);
  }

  int Continent::getID() const
  {
    return *id;
  }

  int Continent::getBonus() const
  {
    return *bonus;
  }

  std::string Continent::getName() const
  {
    return std::string(*name);
  }

  bool Continent::hasTerritory(Territory *territory)
  {
    std::vector<Territory *>::iterator it;
    it = std::find(territories->begin(), territories->end(), territory);

    return it != territories->end();
  }

  void Continent::addTerritory(Territory *territory)
  {
    if (!territory)
      return;
    if (hasTerritory(territory))
      return;

    if (territory->getContinent())
    {
      territory->getContinent()->removeTerritory(territory);
    }

    territory->setContinent(this);
    territories->push_back(territory);
  }

  void Continent::removeTerritory(Territory *territory)
  {
    std::vector<Territory *>::iterator it;
    it = std::find(territories->begin(), territories->end(), territory);

    if (it != territories->end())
    {
      territory->setContinent(nullptr);
      territories->erase(it);
    }
  }

  bool Continent::isConnectedSubGraph()
  {
    std::cout << "CHECKING SUBGRAPH " << *this << std::endl;
    std::unordered_map<int, bool> territory_ids_visited_map = std::unordered_map<int, bool>();
    territory_ids_visited_map.reserve(territories->size());
    for (Territory *territory : *territories)
    {
      territory_ids_visited_map.insert(std::make_pair(territory->getID(), false));
    }

    traverseContinentWithoutLeaving(*(territories->begin()), territory_ids_visited_map);

    return std::all_of(
        territory_ids_visited_map.begin(),
        territory_ids_visited_map.end(),
        [](auto pair) { return pair.second; });
  }

  void Continent::traverseContinentWithoutLeaving(Territory *territory, std::unordered_map<int, bool> &territory_ids_visited_map)
  {
    territory_ids_visited_map[territory->getID()] = true;
    for (Territory *neighbour : territory->getNeighbours())
    {
      if (neighbour->getContinent()->getID() == getID() && !territory_ids_visited_map[neighbour->getID()])
      {
        std::cout << "GOING TO " << *neighbour << " FROM" << *territory << std::endl;
        traverseContinentWithoutLeaving(neighbour, territory_ids_visited_map);
      }
    }
  }

  std::ostream &operator<<(std::ostream &os, const Continent &continent)
  {
    os << continent.toString();
    return os;
  }

  // END OF CONTINENT CLASS DEFINITIONS
} // namespace map