#pragma once
#include <list>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <string>
#include <iostream>
#include <iostream>

using std::ostream;

// These are our observers, that will be notified by the Observable class for any changes. Class is abstract, to implement update method in the children.
class Observer
{
public:
  // destructor
  ~Observer(); 
  // abstract method to be implemented by observers.
  virtual void update() = 0;

protected:
  // default constructor.
  Observer(); 
};

// This is the subject that will notify all observers subscribed to it.
class Observable
{
public:
  // attach observer, will be called to notify.
  virtual void attach(Observer *o);
  // detach observer, will remove from the notification chain.
  virtual void detach(Observer *o); 
  // notifies all observers attached to the Observable.
  virtual void notify(); 
  // default constructor
  Observable(); 
  // destructor
  ~Observable(); 
  // copy constructor
  Observable(const Observable& observableCopy); 

  // Overloads the stream insertion operator.
  friend ostream& operator<<(ostream& out, const Observable& ObservableToStream);

  // Overloads the equals operator
  Observable& operator=(const Observable& o);

private:
  // all observers subscribed to the observable will be stored here.
  std::list<Observer *> *_observers; 
};

// allows for multiple types across the observable and observer to cross reference each other.
template <class T>
class VectorObservable : public Observable, public Observer
{
  // Reference: https://stackoverflow.com/questions/37956299/passing-the-type-of-a-pointer-as-a-template-parameter
  using type = std::remove_pointer_t<T>;
  static_assert(std::is_pointer<T>::value, "T must be of base Observable*");
  static_assert(std::is_base_of<Observable, type>::value, "T must be of base Observable*");

public:
  // default constructor
  VectorObservable(); 
  
  // 1 parameter constructor, accepts vector of template type.
  VectorObservable(std::vector<T> &); 
  // copy constructor
  VectorObservable(const VectorObservable& vectorObservableCopy); 

  // destructor
  ~VectorObservable(); 

  // 1 parameter constructor.
  void set(std::vector<T> &); 
  // getter
  const std::vector<T> &get(); 
  // push_back
  void push_back(T); 
  // remove from template chosen class.
  void remove(T); 
  // to clear observables.
  void clear(); 
  // update method that calls notify, overloading update from observer class. Has to be defined.
  virtual void update(); 

private:
  // clears obsevers.
  void silent_clear();
  // state of observer.
  std::vector<T> state; 
};

// prints messages from observable.
class StringLog : public Observable {

  public:
    // default constructor
    StringLog(); 
    // default constructor
    ~StringLog(); 

    // append the message.
    void append(std::string msg); 
    // clear message.
    void clear();  
    // get message.
    const std::list<std::string>& get(); 
  private:
  std::list<std::string> log;
};

// IMPLEMENTATION FOR TEMPLATE CLASSES
// **(Because template implemenations must be in the same file as the declaration)**
template <class T>
class ConcreteObservable : public Observable {
public:
  // set new state
  void set(T new_state); 
  // default constructor 
  ConcreteObservable(); 
  // copy constructor
  ConcreteObservable(const ConcreteObservable<T>&); 
  T get(); // get state
private:
  T state;
};

// default constructor
template <class T>
ConcreteObservable<T>::ConcreteObservable() {} 

// copy constructor
template <class T>
ConcreteObservable<T>::ConcreteObservable(const ConcreteObservable<T>& copy) {
    state = copy.state;
}

// adds a new state to concrete observable.
template <class T>
void ConcreteObservable<T>::set(T new_state)
{
  state = new_state;
  // notify observers
  notify();
}

// returns the state with the concreteobservable
template <class T>
T ConcreteObservable<T>::get() 
{
  return state;
}

// default constructor implementation.
template <class T>
VectorObservable<T>::VectorObservable() {} 

// destructor
template <class T>
VectorObservable<T>::~VectorObservable() {} 

// sets the state to base
template <class T>
VectorObservable<T>::VectorObservable(std::vector<T>& base) { 
  state = base;
}

// sets an observable to the base.
template <class T>
void VectorObservable<T>::set(std::vector<T> &base) {
  state = base;
  clear();
  for (auto element : state) {
    if (element) {
      element->attach(this);
    }
  }
  notify();
}

// clears all data and notifies observers to do the same.
template <class T>
void VectorObservable<T>::clear() {
  silent_clear();
  notify();
}

// detaches all observers from observable.
template <class T>
void VectorObservable<T>::silent_clear() {
  while (state.size())
  {
    T obs = state.back();
    state.pop_back();
    if (obs) {
      obs->detach(this);
    }
  }
}

// remove element from observable. iterates through state to find it and erases it from state.
template <class T>
void VectorObservable<T>::remove(T el) {
  auto found = std::find(state.begin(), state.end(), el);
  if (found != state.end()) {
    if (*found)
    {
      (*found)->detach(this);
    }
    state.erase(found);
  }
}

// push object into VectorObservable and attach it to the observable. Also push it into the state.
template <class T>
void VectorObservable<T>::push_back(T el) {
  if (el) {
    el->attach(this);
    state.push_back(el);
  }
}

// calls notify method to update all observers.
template <class T>
void VectorObservable<T>::update() {
  notify();
}

// returns the state of the Observable 
template <class T>
const std::vector<T>& VectorObservable<T>::get() {
  return state;
}