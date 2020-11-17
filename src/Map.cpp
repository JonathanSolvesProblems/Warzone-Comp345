#include "Map.h"

namespace map
{

  // START OF MAP DEFINITIONS


  Map::Map()
  {
    territories = new std::vector<Territory *>();
    continents = new std::vector<Continent *>();
  }

  Map::Map(const Map &other)
  {
    territories = new std::vector<Territory *>();
    territories->assign(other.territories->begin(), other.territories->end());

    continents = new std::vector<Continent *>();
    continents->assign(other.continents->begin(), other.continents->end());
  }

  Map::~Map()
  {
  
    while (continents->size())
    {
      Continent* continent = continents->back();
      continents->pop_back();

      for (Territory *territory : continent->getTerritories())
      {
        std::remove_if(
            territories->begin(),
            territories->end(),
            [territory](Territory* t) {
              return t == territory;
            });
        continent->removeTerritory(territory);
        // delete territory;
      }
    }
    continents->clear();
    territories->clear();
    delete continents;
    delete territories;
  }

  void Map::addContinent(Continent *continent)
  {
    if (getContinent(continent->getID()))
      return;
    this->continents->push_back(continent);
  }

  void Map::addTerritory(Territory *territory)
  {
    if (getTerritory(territory->getID()))
      return;

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
    return isConnectedGraph() && everyContinentIsConnectedSubgraph() && eachTerritoryHasSingleContinent();
  }

  bool Map::isConnectedGraph()
  {
#ifdef DEBUG
    std::cout << "CHECKING IF CONNECTED GRAPH" << std::endl;
#endif
    std::unordered_map<int, bool> territory_ids_visited_map = std::unordered_map<int, bool>();
    territory_ids_visited_map.reserve(territories->size());

#ifdef DEBUG
    std::cout << "INSERTING TERRITORIES" << std::endl;
#endif
    for (Territory *territory : *territories)
    {
      territory_ids_visited_map.insert(std::make_pair(territory->getID(), false));
    }
#ifdef DEBUG
    std::cout << "INSERTED TERRITORIES" << std::endl;
    std::cout << "BEGIN " << **(territories->begin()) << std::endl;
#endif
    traverseGraph(*(territories->begin()), territory_ids_visited_map);

#ifdef DEBUG
    std::cout << "TRAVERSED GRAPH" << std::endl;
#endif

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
    std::unordered_set<int> territories_with_continents = std::unordered_set<int>();
    bool valid = true;
    territories_with_continents.reserve(territories->size());
    
    for (Continent* continent : *continents) {
      for (Territory* territory : continent->getTerritories()) {
        if (!territories_with_continents.count(territory->getID())) {
          territories_with_continents.insert(territory->getID());
        } else {
          valid = false;
          goto escape_nested_loop;
        }
      }
    }
    escape_nested_loop:

    return valid;
  }

  void Map::traverseGraph(Territory *territory, std::unordered_map<int, bool> &territory_ids_visited_map)
  {
    territory_ids_visited_map[territory->getID()] = true;
    for (Territory *neighbour : territory->getNeighbours())
    {
      if (!territory_ids_visited_map[neighbour->getID()])
      {
        #ifdef DEBUG
        std::cout << "GOING TO " << *neighbour << " FROM" << *territory << std::endl;
        #endif
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

  // START OF TERRITORY CLASS DEFINITIONS

  Territory::Territory(int id, std::string name, Continent &continent)
  {
    this->id = new int(id);
    this->armees = 0;
    this->name = new std::string(name);
    this->continent = &continent;
    this->continent->addTerritory(this);
    this->neighbours = new std::vector<Territory *>();
  }

  Territory::Territory(const Territory &other)
  {
    this->id = new int(other.getID());
    this->armees = other.getArmees();
    this->name = new std::string(other.getName());
    this->continent = other.continent;
    this->neighbours = new std::vector<Territory *>();
    this->neighbours->assign(other.neighbours->begin(), other.neighbours->end());
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
    neighbours->clear();
    delete neighbours;

    if (continent)
    {
      continent->removeTerritory(this);
    }
  }

  Territory Territory::operator=(const Territory &other) {
    return Territory(other);
  }

  std::string Territory::toString() const
  {
    return "(Territory #" + std::to_string(*id) + ") " + *name + " " + std::to_string(armees) + " armees";
  }

  int Territory::getID() const
  {
    return *id;
  }

  int Territory::getArmees() const
  {
    return armees;
  }

  void Territory::setArmees(int number)
  {
    armees = number;
  }

  std::string Territory::getName() const
  {
    return std::string(*name);
  }

  Continent *Territory::getContinent() const
  {
    return continent;
  }

  Player *Territory::getOwner() const
  {
    return owner_player;
  }

  void Territory::setOwner(Player* player)
  {
    if (this->owner_player) {
      this->owner_player->removeTerritory(this);
    }
    if (player) {
      player->addTerritory(this);
    }
    this->owner_player = player;
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

  // START OF CONTINENT CLASS DEFINITIONS

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
        delete territory;
      }
    }
    territories->clear();
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

  const std::vector<Territory *> Continent::getTerritories() {
    return *territories;
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
#ifdef DEBUG
    std::cout << "CHECKING SUBGRAPH " << *this << std::endl;
#endif
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
#ifdef DEBUG
        std::cout << "GOING TO " << *neighbour << " FROM" << *territory << std::endl;
#endif
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
}