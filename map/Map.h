#pragma once
#define DEBUG // Remove this to disabled debugging
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <utility>

namespace map
{

  class Map;
  class Territory;
  class Continent;

/*
Represents a Warzone Map as a connected graph of territories.
*/
class Map
{

private:
  std::vector<Territory*>* territories{nullptr};
  std::vector<Continent*>* continents{nullptr};

  bool isConnectedGraph();
  bool everyContinentIsConnectedSubgraph();
  bool eachTerritoryHasSingleContinent();

  void traverseGraph(Territory*, std::unordered_map<int, bool> &territory_ids_visited_map);

public:
  Map();
  Map(const Map&);
  ~Map();

  void addContinent(Continent*);
  
  void addTerritory(Territory*);

  void connectTerritories(Territory*, Territory*);
  void connectTerritories(int, int);

  Territory* getTerritory(int) const;
  Territory* getTerritory(std::string) const;

  Continent* getContinent(int) const;
  Continent* getContinent(std::string) const;

  bool validate();

  std::string toString() const;

  friend std::ostream &operator<<(std::ostream &, const Map &);
};

class Territory
{
private:
  int *id{nullptr};
  std::string *name{nullptr};
  Continent *continent{nullptr};
  std::vector<Territory *> *neighbours{nullptr};

  Territory *_findAndEraseNeighbour(Territory *);

public:
  Territory(int, std::string, Continent &);
  ~Territory();

  std::string toString() const;

  int getID() const;
  Continent* getContinent() const;
  std::string getName() const;

  int getNeighbourCount() const;

  Territory *getNeighbour(int) const;
  Territory *getNeighbour(std::string) const;
  const std::vector<Territory *> getNeighbours() const;

  bool hasNeighbour(int) const;
  bool hasNeighbour(Territory *) const;

  Territory *addNeighbour(Territory *);
  void setContinent(Continent*);

  void deleteNeighbour(Territory *);
  void deleteNeighbour(int);
  void deleteNeighbour(std::string);

  friend std::ostream &operator<<(std::ostream &, const Territory &);
};

class Continent
{

private:
  int *id{nullptr};
  std::string *name{nullptr};
  int *bonus{nullptr};
  std::vector<Territory*>* territories;

  void traverseContinentWithoutLeaving(Territory*, std::unordered_map<int, bool>&);

public : Continent(int, std::string, int);
  Continent(const Continent &);
  ~Continent();
  Continent operator=(const Continent &);
  friend std::ostream &operator<<(std::ostream &, const Continent &);

  std::string toString() const;

  int getID() const;
  std::string getName() const;
  int getBonus() const;

  bool hasTerritory(Territory*);
  void addTerritory(Territory*);
  void removeTerritory(Territory*);

  bool isConnectedSubGraph();
};
}