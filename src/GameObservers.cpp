#include "GameObservers.h"



Observer::Observer(){}; 

Observer::~Observer(){}; 

Observable::Observable() 
{
  _observers = new std::list<Observer *>;
}

// copy constructor
Observable::Observable(const Observable &observableCopy) {
    _observers = new std::list<Observer *>();
    _observers->assign(observableCopy._observers->begin(), observableCopy._observers->end());
}

ostream& operator<<(ostream& out, const Observable& ObservableToStream) {
	  return out;
}

Observable& Observable::operator=(const Observable& observableToAssign) {
	return *this;
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
