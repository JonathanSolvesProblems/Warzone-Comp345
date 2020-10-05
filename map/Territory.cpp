#pragma once
#include "Territory.h"

namespace map
{
  Territory::Territory(int id, std::string name, Continent& continent) {
    this->id = new int(id);
    this->name = new std::string(name);
    this->continent = &continent;
    this->neighbours = new std::vector<Territory*>();
  }

  Territory::~Territory() {
    delete id;
    delete name;

    // NOTE: The territories refered to by neighbours will not be deleted,
    //       which means that there will be dangling pointers in the neighbours
    //       vector of each neighbour (because the relationships go both ways).
    for (Territory* neighbour : *neighbours) {
      neighbour->deleteNeighbour(this);
    }
    delete neighbours;
  }

  std::string Territory::toString() const {
    return "(Territory #" + std::to_string(*id) + ") " + *name;
  }

  int Territory::getID() const {
    return *id;
  }

  std::string Territory::getName() const {
    return std::string(*name);
  }

  Continent Territory::getContinent() const {
    return *continent;
  }

  int Territory::getNeighbourCount() const {
    return neighbours->size();
  }

  Territory *Territory::getNeighbour(int neighbour_id) const
  {
    for (Territory* neighbour : *neighbours ) {
      if (neighbour->getID() == neighbour_id)
      {
        return neighbour;
      }
    }

    return nullptr;
  }

  Territory* Territory::getNeighbour(std::string neighbour_name) const
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

  const std::vector<Territory*>* Territory::getNeighbours() const {
    return neighbours;
  }

  bool Territory::hasNeighbour(int neighbour_id) const {
    return getNeighbour(neighbour_id) != nullptr;
  }

  bool Territory::hasNeighbour(Territory* neighbour) const {
    if (neighbour != nullptr) {
      return getNeighbour(neighbour->getID()) == neighbour;
    } else {
      return false;
    }
  }

  Territory* Territory::addNeighbour(Territory* territory){
    if (!hasNeighbour(territory)) {
      neighbours->push_back(territory);
      territory->neighbours->push_back(this);
    }
    return territory;
  }

  Territory* Territory::_findAndEraseNeighbour(Territory* territory) {
    for (auto it = neighbours->begin(); it != neighbours->end(); ++it)
    {
      if ((*it) == territory)
      {
        neighbours->erase(it);
        return (*it);
      }
    }
  }

  void Territory::deleteNeighbour(Territory* territory) {
    Territory* deletedTerritory = _findAndEraseNeighbour(territory);
    if (deletedTerritory) {
      deletedTerritory->_findAndEraseNeighbour(this);
    }
  }

  void Territory::deleteNeighbour(int id) {
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
}
