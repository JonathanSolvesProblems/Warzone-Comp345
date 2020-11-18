#pragma once
#include <list>
#include <vector>
#include <type_traits>
#include <algorithm>

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

private:
  std::list<Observer *> *_observers; // all observers subscribed to the observable will be stored here.
};

// allows for multiple types across the observable and observer.
template <class T>
class VectorObservable : public Observable, public Observer
{
  // Reference: https://stackoverflow.com/questions/37956299/passing-the-type-of-a-pointer-as-a-template-parameter
  using type = std::remove_pointer_t<T>;
  static_assert(std::is_pointer<T>::value, "T must be of base Observable*");
  static_assert(std::is_base_of<Observable, type>::value, "T must be of base Observable*");

public:
  VectorObservable(); // default constructor
  VectorObservable(std::vector<T> &); 
  ~VectorObservable(); // destructor

  void set(std::vector<T> &);
  const std::vector<T> &get();
  void push_back(T);
  void remove(T);
  void clear();
  virtual void update();

private:
  void silent_clear();
  std::vector<T> state;
};

// IMPLEMENTATION FOR TEMPLATE CLASSES
// (Because template implemenations must be in the same file as the declaration)

template <class T>
class ConcreteObservable : public Observable {
public:
  void set(T new_state);
  T get();
private:
  T state;
};

template <class T>
void ConcreteObservable<T>::set(T new_state)
{
  state = new_state;
  notify();
}

template <class T>
T ConcreteObservable<T>::get()
{
  
  return state;
}

template <class T>
VectorObservable<T>::VectorObservable() {}

template <class T>
VectorObservable<T>::~VectorObservable() {}

template <class T>
VectorObservable<T>::VectorObservable(std::vector<T>& base) {
  state = base;
}

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

template <class T>
void VectorObservable<T>::clear() {
  silent_clear();
  notify();
}

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

template <class T>
void VectorObservable<T>::push_back(T el) {
  if (el) {
    el->attach(this);
    state.push_back(el);
  }
}

template <class T>
void VectorObservable<T>::update() {
  notify();
}

template <class T>
const std::vector<T>& VectorObservable<T>::get() {
  return state;
}