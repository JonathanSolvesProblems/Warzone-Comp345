#include "Continent.h"

namespace map
{
    // this is not proper format 
  Continent::Continent(int id, std::string name, int bonus) {
    this->id = new int(id);
    this->name = new std::string(name);
    this->bonus = new int(bonus);
  }

  Continent::Continent(const Continent &other) {
    id = new int(*other.id);
    bonus = new int(*other.bonus);
    name = new std::string(*other.name);
  }

  Continent::~Continent() {
    delete id;
    delete bonus;
    delete name;
  }

  Continent Continent::operator=(const Continent &other)
  {
    return Continent(other);
  }

  std::string Continent::toString() const
  {
    return "(Continent #" + std::to_string(*id) + ") " + *name + " +" + std::to_string(*bonus);
  }

  int Continent::getID() const {
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

  std::ostream& operator<<(std::ostream& os, const Continent& continent)
  {
    os << continent.toString();
    return os;
  }

} // namespace map