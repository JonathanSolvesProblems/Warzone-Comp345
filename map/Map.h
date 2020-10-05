#pragma once
#include <vector>

#include "Territory.h"

namespace map
{

/*
Represents a Warzone Map as a connected graph of territories.
*/
class Map
{

private:
  std::vector<Territory*>* territories{nullptr};
  std::vector<Continent*>* continents{nullptr};

public:
  Map();
  Map(const Map&);
  ~Map();

  void addContinent(Continent*);
  
  void addTerritory(Territory*);

  void connectTerritories(int, int);

  Territory* getTerritory(int) const;
  Territory* getTerritory(std::string) const;

  Continent* getContinent(int) const;
  Continent* getContinent(std::string) const;

  bool validate();

  std::string toString() const;

};

}