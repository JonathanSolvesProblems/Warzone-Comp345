#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "Continent.h"

namespace map
{

  class Territory
  {
  public:
    int* id {nullptr};
    std::string* name {nullptr};
    Continent* continent {nullptr};
    std::vector<Territory*>* neighbours {nullptr};

    Territory* _findAndEraseNeighbour(Territory*);

  public:
    Territory(int, std::string, Continent&);
    ~Territory();

    std::string toString() const;

    int getID() const;
    Continent getContinent() const;
    std::string getName() const;

    int getNeighbourCount() const;

    Territory* getNeighbour(int) const;
    Territory* getNeighbour(std::string) const;
    const std::vector<Territory*>* getNeighbours() const;

    bool hasNeighbour(int) const;
    bool hasNeighbour(Territory*) const;

    Territory* addNeighbour(Territory*);

    void deleteNeighbour(Territory*);
    void deleteNeighbour(int);
    void deleteNeighbour(std::string);

    friend std::ostream& operator<<(std::ostream&, const Territory&);
  };

}