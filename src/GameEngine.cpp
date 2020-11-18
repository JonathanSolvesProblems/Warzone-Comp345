#include "GameEngine.h"
#include <algorithm> // for std::sort

string convertEnum(Phase current_phase)
{
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

MainMenuView::MainMenuView(int w, int h, GameModel *mgm) : WindowView(w, h, (COLS - w) / 2, (LINES - h) / 2)
{
  _settings_model = mgm;
  _settings_model->phase_headers_enabled.attach(this);
  _settings_model->stats_headers_enabled.attach(this);
  _settings_model->number_of_players.attach(this);
};

MainMenuView::~MainMenuView()
{
  _settings_model->phase_headers_enabled.detach(this);
  _settings_model->stats_headers_enabled.detach(this);
  _settings_model->number_of_players.detach(this);
}

void MainMenuView::display_banner(int &offset)
{
  wattron(_window, COLOR_PAIR(RED_BLACK));
  offset += 2;
  wmove(_window, offset, 0);
  wprintw(_window,
          R"( __      __                                          
/  \    /  \_____ ___________________   ____   ____  
\   \/\/   /\__  \\_  __ \___   /  _ \ /    \_/ __ \ 
 \        /  / __ \|  | \//    (  <_> )   |  \  ___/ 
  \__/\  /  (____  /__|  /_____ \____/|___|  /\___  >
       \/        \/            \/          \/     \/ )");
  offset = getcury(_window);
  wattroff(_window, COLOR_PAIR(RED_BLACK));
}

void MainMenuView::display_credits(int &offset)
{
  wattron(_window, COLOR_PAIR(GREY_BLACK));
  print_centered(height - 4, "Created by");
  wattron(_window, COLOR_PAIR(WHITE_BLACK));
  print_centered(height - 3, "Alex Kofman    Anthony van Voorst    Brendan Bissessar");
  print_centered(height - 2, "Drew Wagner    Jonathan Andrei");
  wattroff(_window, COLOR_PAIR(WHITE_BLACK));
}

void MainMenuView::display_menu(int &offset)
{
  wattron(_window, COLOR_PAIR(RED_BLACK));
  print_centered(offset + 3, "Number of Players (UP/DOWN):  ");
  wattron(_window, COLOR_PAIR(WHITE_BLACK));
  print_centered(offset + 5, std::to_string(_settings_model->number_of_players.get()).c_str());
  wattron(_window, COLOR_PAIR(RED_BLACK));
  print_centered(offset + 7, "Press SPACE to begin");
  wattroff(_window, COLOR_PAIR(RED_BLACK));

  if (_settings_model->phase_headers_enabled.get())
  {
    wattron(_window, COLOR_PAIR(BLACK_GREEN));
    print_centered(offset + 9, " PHASE HEADER ENABLED  (p) ");
    wattroff(_window, COLOR_PAIR(BLACK_GREEN));
  }
  else
  {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(offset + 9, " PHASE HEADER DISABLED (p) ");
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }

  if (_settings_model->stats_headers_enabled.get())
  {
    wattron(_window, COLOR_PAIR(BLACK_GREEN));
    print_centered(offset + 11, " STATS HEADER ENABLED  (o) ");
    wattroff(_window, COLOR_PAIR(BLACK_GREEN));
  }
  else
  {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(offset + 11, " STATS HEADER DISABLED (o) ");
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }

  offset = getcury(_window);
}

void MainMenuView::display()
{
  if (!_window)
  {
    activate();
  }

  int offset = 0;
  wclear(_window);
  display_banner(offset);
  display_menu(offset);
  display_credits(offset);
  WindowView::display();
}

void MainMenuView::update()
{
  display();
}

MainMenuController::MainMenuController(GameModel *mgm)
{
  _settings_model = mgm;
}

MainMenuController::~MainMenuController()
{
}

bool MainMenuController::keyboardEventPerformed(int key)
{
  if (key == 'p')
  {
    bool current = _settings_model->phase_headers_enabled.get();
    _settings_model->phase_headers_enabled.set(!current);
    return true;
  }
  else if (key == 'o')
  {
    bool current = _settings_model->stats_headers_enabled.get();
    _settings_model->stats_headers_enabled.set(!current);
    return true;
  }
  else if (key == KEY_UP || key == 'w')
  {
    int nNumberOfPlayers = _settings_model->number_of_players.get() + 1;
    if (nNumberOfPlayers > 5)
      nNumberOfPlayers = 5;
    _settings_model->number_of_players.set(nNumberOfPlayers);
  }
  else if (key == KEY_DOWN || key == 's')
  {
    int nNumberOfPlayers = _settings_model->number_of_players.get() - 1;
    if (nNumberOfPlayers < 2)
      nNumberOfPlayers = 2;
    _settings_model->number_of_players.set(nNumberOfPlayers);
  }
  else if (key == ' ')
  {
    Application::instance()->activateView(MAP_SELECTION_VIEW);
    return true;
  }
  return false;
}

void MainMenuController::viewActivated() {}
void MainMenuController::viewDeactivated() {}

MapMenuView::MapMenuView(int w, int h, MenuModel *mm) : WindowView(w, h, (COLS - w) / 2, LINES - h)
{
  _menu_model = mm;
  _menu_model->map_file_list.attach(this);
  _menu_model->selected_index.attach(this);
  _menu_model->error_message.attach(this);
}

MapMenuView::~MapMenuView()
{
  _menu_model->map_file_list.detach(this);
  _menu_model->selected_index.detach(this);
  _menu_model->error_message.detach(this);
}

void MapMenuView::update()
{
  display();
}

void MapMenuView::display()
{
  if (!_window)
  {
    activate();
  }

  wclear(_window);
  wattron(_window, COLOR_PAIR(RED_BLACK));
  box(_window, 0, 0);
  wattroff(_window, COLOR_PAIR(RED_BLACK));

  std::vector<std::string> maps = _menu_model->map_file_list.get();
  int active_index = _menu_model->selected_index.get();
  for (int i = 0; i < maps.size(); i++)
  {
    if (i == active_index)
    {
      wattron(_window, COLOR_PAIR(BLACK_RED));
      print_centered(1 + i * 2, maps[i]);
      wattroff(_window, COLOR_PAIR(BLACK_RED));
    }
    else
    {
      print_centered(1 + i * 2, maps[i]);
    }
  }
  if (_menu_model->error_message.get() != "")
  {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(height / 2, _menu_model->error_message.get());
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }

  WindowView::display();
}

MapSelectionView::MapSelectionView(int w, int h, int header_height, MenuModel *mm) : WindowView(w, header_height, (COLS - w) / 2, 0)
{
  _menu_view = new MapMenuView(w, h - header_height, mm);
}

MapSelectionView::~MapSelectionView()
{
  delete _menu_view;
}

void MapSelectionView::notifyKeyboardEventPerformed(int key)
{
  // Send everything except SPACE To the sub view
  if (key != ' ' && key != KEY_BACKSPACE)
  {
    _menu_view->notifyKeyboardEventPerformed(key);
  }
  else
  {
    View::notifyKeyboardEventPerformed(key);
  }
}

void MapSelectionView::registerMenuListener(ActionListener *listener)
{
  _menu_view->registerListener(listener);
}

void MapSelectionView::activate()
{
  WindowView::activate();
  _menu_view->activate();
}

void MapSelectionView::deactivate()
{
  WindowView::deactivate();
  _menu_view->deactivate();
}

void MapSelectionView::display()
{
  if (!_window)
  {
    activate();
  }
  wclear(_window);
  wattron(_window, COLOR_PAIR(RED_BLACK));
  print_centered(2, "Please select a map from the list below:");
  wattroff(_window, COLOR_PAIR(RED_BLACK));
  wattron(_window, COLOR_PAIR(GREY_BLACK));
  print_centered(3, "Use UP / DOWN to move between selections");
  print_centered(4, "Press SPACE to finalize selection");
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  _menu_view->display();
  WindowView::display();
}

void MenuModel::incrementItem(int inc)
{
  int current = selected_index.get();
  selected_index.set((current + inc) % map_file_list.get().size());
}

std::string MenuModel::getSelection()
{
  return map_file_list.get()[selected_index.get()];
}

MapMenuController::MapMenuController(MenuModel *mm)
{
  _menu_model = mm;
}

MapMenuController::~MapMenuController() {}

bool MapMenuController::keyboardEventPerformed(int key)
{
  if (key == KEY_UP || key == 'w')
  {
    _menu_model->error_message.set("");
    _menu_model->incrementItem(-1);
  }
  else if (key == KEY_DOWN || key == 's')
  {
    _menu_model->error_message.set("");
    _menu_model->incrementItem(1);
  }
  else
  {
    return false;
  }
  return true;
}

void MapMenuController::viewActivated() {}
void MapMenuController::viewDeactivated() {}

MapSelectionController::MapSelectionController(MenuModel *mm, GameModel *gm)
{
  _menu_model = mm;
  _game_model = gm;
}

MapSelectionController::~MapSelectionController() {}

bool MapSelectionController::keyboardEventPerformed(int key)
{
  if (key == ' ')
  {
    // Load map file
    // this is the string that gets the map file

    // if this works then load the file
    // else you have to prompt an error message in a screen
    std::string map_file = _menu_model->getSelection();

    MapLoader mapLoader;

    if (_game_model->map)
    {
      delete _game_model->map;
    }

    _game_model->map = new map::Map();

    if (mapLoader.loadFile(map_file, *_game_model->map) && _game_model->map->validate())
    {
      // load file under
      Application::instance()->activateView(GAMEPLAY_VIEW);
    }
    else
    {
      _menu_model->error_message.set("This is a invalid file! Please choose another!");
    }

    return true;
  }
  return false;
}

void MapSelectionController::viewActivated()
{
  map::Map map;
  MapLoader maploader;

  std::vector<std::string> map_list = maploader.findMapFiles();
  _menu_model->map_file_list.set(map_list);
}
void MapSelectionController::viewDeactivated() {}

PhaseObserverView::PhaseObserverView(int w, int h, int x, int y, GameModel *gm) : WindowView(w, h, x, y)
{
  _game_model = gm;
  _game_model->current_player.attach(this);
  _game_model->current_phase.attach(this);
}

StatisticsObserverView::StatisticsObserverView(int w, int h, int x, int y, GameModel *gm) : WindowView(w, h, x, y)
{
  _game_model = gm;
  _game_model->active_players.attach(this);
}

void PhaseObserverView::display()
{
  wclear(_window);
  box(_window, 0, 0);
  Player *current_player = _game_model->current_player.get();
  if (current_player != nullptr)
  {
    print_centered(height / 2, current_player->playerName);
  }

  print_centered(height / 4, convertEnum(_game_model->current_phase.get()));

  WindowView::display();
}
// do phase after.
void StatisticsObserverView::display()
{
  wclear(_window);
  box(_window, 0, 0);

  std::vector<Player *> players = _game_model->active_players.get();

  if (players.size() == 1)
  {
    // draw the winning message.
    print_centered(height / 2, "Congratulations! " + players[0]->playerName + " has won the game!");
  }
  else
  {
      // sorting the players in descending order, based on how many territories they own.

      // _game_model->active_players.begin(), _game_model-
      // 

      std::vector<Player*> sortedPlayers;
      

      sortedPlayers.assign(players.begin(), players.end());

      std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](auto playerA, auto playerB){
          return playerA->owned_territories.size() > playerB->owned_territories.size();
      });

    int row = 0;
    // else draw the graphic.
    for (auto player : sortedPlayers)
    {

      // gets the # of territories the player owns.
      int number_of_territories = player->owned_territories.size();

      // percentage that the player owns for their territories.
      float percent_owned = (float)number_of_territories / _game_model->map->getTerritories().size();

      int cp;

      
      // for future, need to be relative to size of player name.
      // if the player does not own anymore territories. Remove them from the statistics screen.
     
        // Order players by number of territories they own.
      wmove(_window, 1 + 2 * row++, 1);
      wprintw(_window, player->playerName.c_str());
      waddch(_window, ' ');
      

      if (percent_owned > 0.2)
      {
        cp = COLOR_PAIR(BLACK_GREEN);
      }
      else
      {
        cp = COLOR_PAIR(BLACK_RED);
      }

      wattron(_window, cp);
      for (int j = 0; j < width / 2 * percent_owned; j++)
      {
        waddch(_window, ' ');
      }
      wattroff(_window, cp);
    }
  }

  WindowView::display();
}

void PhaseObserverView::update()
{
  display();
}

void StatisticsObserverView::update()
{
  display();
}

PhaseObserverView::~PhaseObserverView()
{
  _game_model->current_player.detach(this);
  _game_model->current_phase.detach(this);
}

StatisticsObserverView::~StatisticsObserverView()
{
  _game_model->active_players.detach(this);
}

GameplayView::GameplayView(int w, int h, GameModel *sm)
{
  settings_model = sm;
  bool headers_enabled = settings_model->phase_headers_enabled.get() || settings_model->stats_headers_enabled.get();
  this->start_x = 1;
  this->start_y = headers_enabled * LINES / 4;
  this->height = h - start_y;
  this->width = w;
}

GameplayView::~GameplayView()
{
  if (_phase_view)
    delete _phase_view;
  if (_stats_view)
    delete _stats_view;
}

void GameplayView::create_phase_observer_view(int header_height)
{
  _phase_view = new PhaseObserverView(COLS / 2 - 1, header_height, 1, 1, settings_model);
}

void GameplayView::create_stats_observer_view(int header_height)
{
  _stats_view = new StatisticsObserverView(COLS / 2 - 1, header_height, COLS / 2, 1, settings_model);
}

void GameplayView::display() {
  if (!_window) {
    activate();
  }
  if (_phase_view) {
    _phase_view->display();
  }
  if (_stats_view)
  {
    _stats_view->display();
  }

  wclear(_window);
  box(_window, 0, 0);

  print_centered(height / 2, "MAIN CONTENT");
  WindowView::display();
}

void GameplayView::activate()
{
  bool headers_enabled = settings_model->phase_headers_enabled.get() || settings_model->stats_headers_enabled.get();
  int header_height = settings_model->number_of_players.get() * 2 + 1;
  this->start_y = 1 + headers_enabled * header_height;
  this->height = LINES - start_y;

  if (_window && _window != stdscr)
  {
    deactivate();
  }
  _window = create_newwin(height, width, start_y, start_x);
  box(_window, 0, 0);
  display();

  if (settings_model->phase_headers_enabled.get()) {
    create_phase_observer_view(header_height);
    _phase_view->activate();
  }

  if (settings_model->stats_headers_enabled.get())
  {
    create_stats_observer_view(header_height);
    _stats_view->activate();
  }
  View::activate();
}

void GameplayView::deactivate()
{
  WindowView::deactivate();
  if (_phase_view)
  {
    _phase_view->deactivate();
  }
  if (_stats_view)
  {
    _stats_view->deactivate();
  }
}

GameplayController::GameplayController(GameModel *gm)
{
  _game_model = gm;
}

GameplayController::~GameplayController()
{

}

void GameplayController::viewActivated()
{
  startupPhase();
  mainGameLoop();
}

bool GameplayController::keyboardEventPerformed(int key) { return false; }

void GameplayController::startupPhase()
{
  _game_model->current_phase.set(STARTUP);

  int num_players = _game_model->number_of_players.get();

  srand(time(NULL));

  std::vector<Player *> newly_created_players;
  for (int i = 0; i < num_players; i++)
  {
    Player *new_player = new Player("Player " + std::to_string(i + 1), i);
    newly_created_players.push_back(new_player);
  }

  while (!newly_created_players.empty())
  {
    int index = rand() % newly_created_players.size();

    Player *next = newly_created_players[index];
    _game_model->active_players.push_back(next);

    newly_created_players.erase(newly_created_players.begin() + index);
  }

  Player *first = _game_model->active_players.get()[0];
  _game_model->current_player.set(first);

  // for testing winning condition.
  // _game_model->active_players.remove(_game_model->active_players.get()[0]);

  assign_territories();
}

void GameplayController::assign_territories()
{
  std::vector<map::Territory *> territories;
  const std::vector<map::Territory *> original_territories = _game_model->map->getTerritories();
  territories.assign(
      original_territories.begin(),
      original_territories.end());

  int index_of_next_player_to_receive_territory = 0;
  while (!territories.empty())
  {
    int r;
#ifdef __linux__
    r = random();
#else
    r = rand();
#endif
    int territory_index = r % territories.size();

    map::Territory *territory = territories[territory_index];
    Player *player = _game_model->active_players.get()[index_of_next_player_to_receive_territory];
    player->addTerritory(territory);

    territories.erase(territories.begin() + territory_index);

    index_of_next_player_to_receive_territory = ++index_of_next_player_to_receive_territory % _game_model->number_of_players.get();

#ifdef __linux__
    usleep(10000);
#else
    Sleep(100);
#endif
  }

}

void GameplayController::mainGameLoop()
{
  _game_model->current_phase.set(REINFORCEMENT);

  /* Play out game */

  // Here to implement the logic for removing players if they no longer own anymore territories. Check with Anthony first.
        // _game_model->active_players.remove(_game_model->current_player.get()); 

  // Application::instance()->activateView(MAIN_MENU_VIEW);
}

void GameplayController::viewDeactivated()
{
  delete _game_model->map;

  for (auto player : _game_model->active_players.get())
  {
    delete player;
  }

  // TODO We might want to remove these two lines
  // _game_model->active_players.clear();
  // _game_model->current_player.set(nullptr);
}