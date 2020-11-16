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

PhaseObserver::PhaseObserver() {};
PhaseObserver::~PhaseObserver() {};
void PhaseObserver::update() {};
void PhaseObserver::display() {
  // display phase information: 
  // hardcoded test case.
  std::cout << "Player 3: Reinforcement phase or Player 1: Issue orders phase.";

  // call getters from other parts to display them here. Need to get cursor to lineup with the window.
};

StatisticsObserver::StatisticsObserver() {};
StatisticsObserver::~StatisticsObserver() {};
void StatisticsObserver::update() {};
void StatisticsObserver::display() {
  // figuring out a way to display a graph to depict which percentage of territories are owned by which players.
  // update when country has been conquered by player.

  // when player eliminated, remove from view.

  // when player wins, update it, so that just shows that player and a winning message.

};
