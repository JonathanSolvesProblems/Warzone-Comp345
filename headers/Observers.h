#pragma once
#include <list>

class Observer
{
public:
  ~Observer();
  virtual void update() = 0;

protected:
  Observer();
};

class Observable
{
public:
  virtual void attach(Observer *o);
  virtual void detach(Observer *o);
  virtual void notify();
  Observable();
  ~Observable();

private:
  std::list<Observer *> *_observers;
};

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