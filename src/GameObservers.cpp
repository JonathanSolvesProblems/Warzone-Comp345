#include "GameObservers.h"
#include <iostream>


Observer::Observer(){};

Observer::~Observer(){};

Observable::Observable()
{
  _observers = new std::list<Observer *>;
}

Observable::~Observable()
{
  delete _observers;
}

void Observable::attach(Observer *ob)
{
  _observers->push_back(ob);
}

void Observable::detach(Observer *ob)
{
  _observers->remove(ob);
}

void Observable::notify()
{
  for (auto ob : *_observers)
  {
    ob->update();
  }
}

StringLog::StringLog() {}

StringLog::~StringLog() {}

void StringLog::append(std::string msg) {
  log.push_front(msg);
  notify();
}

void StringLog::clear() {
  log.clear();
  notify();
}

const std::list<std::string>& StringLog::get() {
  return log;
}
