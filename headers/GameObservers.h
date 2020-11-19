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
  ~Observer(); // destructor
  virtual void update() = 0; // abstract method to be implemented by observers.

protected:
  Observer(); // default constructor.
};

// This is the subject that will notify all observers subscribed to it.
class Observable
{
public:
  virtual void attach(Observer *o); // attach observer, will be called to notify.
  virtual void detach(Observer *o); // detach observer, will remove from the notification chain.
  virtual void notify(); // notifies all observers attached to the Observable.
  Observable(); // default constructor
  ~Observable(); // destructor.
  Observable(const Observable& observableCopy); // copy constructor

  // Overloads the stream insertion operator.
  friend ostream& operator<<(ostream& out, const Observable& ObservableToStream);

  // Overloads the equals operator
  Observable& operator=(const Observable& o);

private:
  std::list<Observer *> *_observers; // all observers subscribed to the observable will be stored here.
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
  VectorObservable(); // default constructor
  
 
  VectorObservable(std::vector<T> &); // 1 parameter constructor, accepts vector of template type.
  VectorObservable(const VectorObservable& vectorObservableCopy); // copy constructor

  ~VectorObservable(); // destructor

  void set(std::vector<T> &); // 1 parameter constructor.
  const std::vector<T> &get(); // getter
  void push_back(T); // push_back
  void remove(T); // remove from template chosen class.
  void clear(); // to clear observables.
  virtual void update(); // update method that calls notify, overloading update from observer class. Has to be defined.

private:
  void silent_clear();// clears obsevers.
  std::vector<T> state; // state of observer.
};

// prints messages from observable.
class StringLog : public Observable {

  public:
    StringLog(); // default constructor
    ~StringLog(); // default constructor

    void append(std::string msg); // append the message.
    void clear();  // clear message.
    const std::list<std::string>& get(); // get message.

  private:
  std::list<std::string> log;
};

// IMPLEMENTATION FOR TEMPLATE CLASSES
// **(Because template implemenations must be in the same file as the declaration)**
template <class T>
class ConcreteObservable : public Observable {
public:
  void set(T new_state);
  // ConcreteObservable<T>(const ConcreteObservable<T>&);
  T get();
private:
  T state;
};

/*template <class T>
ConcreteObservable::ConcreteObservable<T>(const ConcreteObservable<T>& copy) {
    state = copy.state;
}*/

// adds a new state to concrete observable.
template <class T>
void ConcreteObservable<T>::set(T new_state)
{
  state = new_state;
  notify();
}

template <class T>
T ConcreteObservable<T>::get() // returns the state with the concreteobservable
{
  return state;
}

template <class T>
VectorObservable<T>::VectorObservable() {} // default constructor implementation.

template <class T>
VectorObservable<T>::~VectorObservable() {} // destructor

template <class T>
VectorObservable<T>::VectorObservable(std::vector<T>& base) { // sets the state to base
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