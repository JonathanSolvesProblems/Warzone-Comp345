#pragma once
#include <string>
#include <iostream>

namespace map {

class Continent {

private:
  int *id{nullptr};
  std::string *name{nullptr};
  int *bonus{nullptr};

public:
  Continent(int, std::string, int);
  Continent(const Continent&);
  ~Continent();
  Continent operator=(const Continent&);
  friend std::ostream& operator<<(std::ostream&, const Continent&);

  std::string toString() const;

  int getID() const;
  std::string getName() const;
  int getBonus() const;
};

}