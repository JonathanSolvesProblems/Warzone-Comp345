#include "GameModels.h"

// checks the current phase of the game for N curses
string convertEnum(Phase current_phase)
{
  // a switch statement that checks the current phase and sets it
  switch (current_phase)
  {
  case STARTUP:
    return "Startup Phase";
  case REINFORCEMENT:
    return "Reinforcement Phase";
  case ISSUE_ORDERS:
    return "Issue Orders Phase";
  case ORDERS_EXECUTION:
    return "Orders Execution Phase";
  default:
    return "Unknown Phase";
  }
}

// sets the observable message log
GameModel::GameModel()
{
  log = new StringLog();
  phase_headers_enabled = new ConcreteObservable<bool>(true);
  stats_headers_enabled = new ConcreteObservable<bool>(true);
  number_of_players = new ConcreteObservable<int>(2);

  current_phase = new ConcreteObservable<Phase>(STARTUP);
  current_player = new ConcreteObservable<Player *>();
  active_players = new VectorObservable<Player *>();

  current_order_type = new ConcreteObservable<OrderType>();
  current_step = new ConcreteObservable<int>(-1);
  selected_index = new ConcreteObservable<int>(0);
  territory_list_items = new VectorObservable<ConcreteObservable<std::pair<map::Territory *, int>> *>();
  error_message = new ConcreteObservable<std::string>("");
}

// deletes the observable message log
GameModel::~GameModel()
{
  delete log;
  delete phase_headers_enabled;
  delete stats_headers_enabled;
  delete number_of_players;

  delete current_phase;
  delete current_player;
  delete active_players;

  delete current_order_type;
  delete current_step;
  delete selected_index;
  delete territory_list_items;
  delete error_message;
}
