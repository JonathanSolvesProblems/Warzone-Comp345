#pragma once

#include "GameObservers.h"
#include <string>

class GameModel;

#include "Map.h"

// All possible game phases
enum Phase
{
  STARTUP,
  REINFORCEMENT,
  ISSUE_ORDERS,
  ORDERS_EXECUTION
};

enum OrderType
{
  PASS,
  DEPLOY,
  ADVANCE,
  AIRLIFT,
  BLOCKADE,
  BOMB,
  NEGOTIATE
};

// Converts Phase enum to a std::string for display
std::string convertEnum(Phase current_phase);

/* A POD to store Observables used by the game */
struct GameModel
{
  GameModel();
  ~GameModel();

  // Basic Settings
  ConcreteObservable<bool> *phase_headers_enabled{nullptr};
  ConcreteObservable<bool> *stats_headers_enabled{nullptr};
  ConcreteObservable<int> *number_of_players{nullptr};

  // Tracks the current game phase
  ConcreteObservable<Phase> *current_phase{nullptr};

  // Order type choosen by Human player
  ConcreteObservable<OrderType> *current_order_type{nullptr};

  // Current step of the order issuing process (-1 == automonous order making)
  ConcreteObservable<int> *current_step{nullptr};

  // Currently selected line of the order issuing selector
  ConcreteObservable<int> *selected_index{nullptr};
  // Used to display a list of territories and army numbers during order issuing
  VectorObservable<ConcreteObservable<std::pair<map::Territory *, int>> *> *territory_list_items;
  // Used to show an error message to the human player if they do something wrong while issuing orders
  ConcreteObservable<std::string> *error_message{nullptr};

  /*
   * Tracks the current player.
   * Note: It only updates when the pointer changes, not when
   * the contents of the Player object change
   */
  ConcreteObservable<Player *> *current_player{nullptr};

  /*
   * Tracks the currently active players.
   * Updates when the underlying vector changes, i.e adding/removing
   * AND when the Players themselves change.
   */
  VectorObservable<Player *> *active_players{nullptr};

  /*
   * Not Observable, but necessary for gameplay none the less
   */
  map::Map *map{nullptr};

  /*
   * An observable message log
   */
  StringLog *log{nullptr};
};
