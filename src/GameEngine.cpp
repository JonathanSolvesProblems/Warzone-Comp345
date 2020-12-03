#include "GameEngine.h"


//creates what we see in the main menu 
MainMenuView::MainMenuView(int w, int h, GameModel *mgm) : WindowView(w, h, (COLS - w) / 2, (LINES - h) / 2)
{
  _settings_model = mgm;
  _settings_model->phase_headers_enabled->attach(this);
  _settings_model->stats_headers_enabled->attach(this);
  _settings_model->number_of_players->attach(this);
};

//removes (detaches) all the headers, stats and the number of players in the main menu view
MainMenuView::~MainMenuView()
{
  _settings_model->phase_headers_enabled->detach(this);
  _settings_model->stats_headers_enabled->detach(this);
  _settings_model->number_of_players->detach(this);
}

// displays the main banner on the menu screen
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

// displays all the group members invovled in the project
void MainMenuView::display_credits(int &offset)
{
  wattron(_window, COLOR_PAIR(GREY_BLACK));
  print_centered(height - 4, "Created by");
  wattron(_window, COLOR_PAIR(WHITE_BLACK));
  print_centered(height - 3, "Alex Kofman    Anthony van Voorst    Brendan Bissessar");
  print_centered(height - 2, "Drew Wagner    Jonathan Andrei");
  wattroff(_window, COLOR_PAIR(WHITE_BLACK));
}

// displays the contents of the menu such as the number of players and pressing space to begin the game 
// it also displays the different observables that the players might want to use while playing the game 
void MainMenuView::display_menu(int &offset)
{
  wattron(_window, COLOR_PAIR(RED_BLACK));
  print_centered(offset + 3, "Number of Players (UP/DOWN):  ");
  wattron(_window, COLOR_PAIR(WHITE_BLACK));
  print_centered(offset + 5, std::to_string(_settings_model->number_of_players->get()).c_str());
  wattron(_window, COLOR_PAIR(RED_BLACK));
  print_centered(offset + 7, "Press SPACE to begin");
  wattroff(_window, COLOR_PAIR(RED_BLACK));

  if (_settings_model->phase_headers_enabled->get())
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

  if (_settings_model->stats_headers_enabled->get())
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

//displays the main menu on the screen 
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

// updatest the display window 
void MainMenuView::update()
{
  display();
}

// sets the model settings to mgm in the Main Menu Controller
MainMenuController::MainMenuController(GameModel *mgm)
{
  _settings_model = mgm;
}

MainMenuController::~MainMenuController()
{
}

// controls how the user interacts with the GUI through the keyboard 
bool MainMenuController::keyboardEventPerformed(int key)
{
  // if p is pressed then the settings for the phase headers are enabled or disabled
  if (key == 'p')
  {
    bool current = _settings_model->phase_headers_enabled->get();
    _settings_model->phase_headers_enabled->set(!current);
    return true;
  }
    // if o is pressed then the settings for the stats are enabled or disabled
  else if (key == 'o')
  {
    bool current = _settings_model->stats_headers_enabled->get();
    _settings_model->stats_headers_enabled->set(!current);
    return true;
  }
    // if w or up is pressed then increment number of players 
  else if (key == KEY_UP || key == 'w')
  {
    int nNumberOfPlayers = _settings_model->number_of_players->get() + 1;
    if (nNumberOfPlayers > 5)
      nNumberOfPlayers = 5;
    _settings_model->number_of_players->set(nNumberOfPlayers);
  }
    // if s or down is pressed then increment number of players 
  else if (key == KEY_DOWN || key == 's')
  {
    int nNumberOfPlayers = _settings_model->number_of_players->get() - 1;
    if (nNumberOfPlayers < 2)
      nNumberOfPlayers = 2;
    _settings_model->number_of_players->set(nNumberOfPlayers);
  }
    // if space is pressed then proceed to the map selection
  else if (key == ' ')
  {
    Application::instance()->activateView(MAP_SELECTION_VIEW);
    return true;
  }
  return false;
}

void MainMenuController::viewActivated() {}
void MainMenuController::viewDeactivated() {}

// creates the map menu window 
MapMenuView::MapMenuView(int w, int h, MenuModel *mm) : WindowView(w, h, (COLS - w) / 2, LINES - h)
{
  _menu_model = mm;
  _menu_model->map_file_list->attach(this);
  _menu_model->selected_index->attach(this);
  _menu_model->error_message->attach(this);
}

// destructor for the map menu window 
MapMenuView::~MapMenuView()
{
  _menu_model->map_file_list->detach(this);
  _menu_model->selected_index->detach(this);
  _menu_model->error_message->detach(this);
}

// updates what is displayed on the map menu window 
void MapMenuView::update()
{
  display();
}

// displays the map menu window 
void MapMenuView::display()
{
  if (!_window)
  {
    activate();
  }

// creates the red sqaure with the black background
  wclear(_window);
  wattron(_window, COLOR_PAIR(RED_BLACK));
  box(_window, 0, 0);
  wattroff(_window, COLOR_PAIR(RED_BLACK));

  std::vector<std::string> maps = _menu_model->map_file_list->get();
  int active_index = _menu_model->selected_index->get();
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
  if (_menu_model->error_message->get() != "")
  {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(height / 2, _menu_model->error_message->get());
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }

  WindowView::display();
}

// sets the size of the screen for the map selection window 
MapSelectionView::MapSelectionView(int w, int h, int header_height, MenuModel *mm) : WindowView(w, header_height, (COLS - w) / 2, 0)
{
  _menu_view = new MapMenuView(w, h - header_height, mm);
}

// deconstructor for MapSelectionView
MapSelectionView::~MapSelectionView()
{
  delete _menu_view;
}

// notifies the map selection window if the space bar or back space was pressed
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

// registers all the listeners to the map selection window 
void MapSelectionView::registerMenuListener(ActionListener *listener)
{
  _menu_view->registerListener(listener);
}

// Notifies the MapMenuView that it has been activated
void MapSelectionView::activate()
{
  WindowView::activate();
  _menu_view->activate();
}

// Notifies the MapMenuView that it has been deactivated 
void MapSelectionView::deactivate()
{
  WindowView::deactivate();
  _menu_view->deactivate();
}

 // Draws all the content for the map selection view that the user sees as well as the instructions on how to select a map file
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

MenuModel::MenuModel() {
  map_file_list = new ConcreteObservable<std::vector<std::string>>();
  selected_index = new ConcreteObservable<int>();
  error_message = new ConcreteObservable<std::string>();
}

MenuModel::~MenuModel() {
  delete map_file_list;
  delete selected_index;
  delete error_message;
}

void MenuModel::incrementItem(int inc)
{
  int current = selected_index->get();
  selected_index->set((current + inc) % map_file_list->get().size());
}

// Gets the name of the currently selected file
std::string MenuModel::getSelection()
{
  return map_file_list->get()[selected_index->get()];
}

// Obtains a reference to menu model 
MapMenuController::MapMenuController(MenuModel *mm)
{
  _menu_model = mm;
}

// Destructor for the MapMenuController 
MapMenuController::~MapMenuController() {}

// Increments ot decrements a selected index when the up,w,down or s keys are pressed 
bool MapMenuController::keyboardEventPerformed(int key)
{
  if (key == KEY_UP || key == 'w')
  {
    _menu_model->error_message->set("");
    _menu_model->incrementItem(-1);
  }
  else if (key == KEY_DOWN || key == 's')
  {
    _menu_model->error_message->set("");
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

// Obtain references to the necessary Menu Model and Game Model
MapSelectionController::MapSelectionController(MenuModel *mm, GameModel *gm)
{
  _menu_model = mm;
  _game_model = gm;
}

MapSelectionController::~MapSelectionController() {}

// if space is pressed on the desired file, it will test to see if it is valid, if it is the file will load
// if not, the file will be gracefully rejected 
bool MapSelectionController::keyboardEventPerformed(int key)
{
  // if space is pressed 
  if (key == ' ')
  {
    //validate and load file
    std::string map_file = _menu_model->getSelection();
    MapLoader mapLoader;
    ConquestFileReaderAdapter conquestFileReader;

    if (_game_model->map)
    {
      delete _game_model->map;
    }

    _game_model->map = new map::Map();

    if (mapLoader.loadFile(map_file, *_game_model->map)&& _game_model->map->validate())
    {
      // load file under
      Application::instance()->activateView(GAMEPLAY_VIEW);
    }

           if (conquestFileReader.loadFile(map_file, *_game_model->map)) {
            // load file under
                Application::instance()->activateView(GAMEPLAY_VIEW);      
        }
    


        else
    {
      _menu_model->error_message->set("This is a invalid file! Please choose another!");
    }
    

    // if the file is not valid output an error message
    

    return true;
  }
  return false;
}

void MapSelectionController::viewActivated()
{
  // creates objects of a map and maploader and then retrieves all the map files in a directory in a list of vectors
  map::Map map;
  MapLoader maploader;

  std::vector<std::string> map_list = maploader.findMapFiles();
  _menu_model->map_file_list->set(map_list);
}
void MapSelectionController::viewDeactivated() {}

// Displays the current game phase and the name of the current player
PhaseObserverView::PhaseObserverView(int w, int h, int x, int y, GameModel *gm) : WindowView(w, h, x, y)
{
  _game_model = gm;
  _game_model->current_player->attach(this);
  _game_model->current_phase->attach(this);
}

StatisticsObserverView::StatisticsObserverView(int w, int h, int x, int y, GameModel *gm) : WindowView(w, h, x, y)
{
  _game_model = gm;
  _game_model->active_players->attach(this);
}

void PhaseObserverView::display()
{
  wclear(_window);
  box(_window, 0, 0);
  Player *current_player = _game_model->current_player->get();
  if (current_player != nullptr)
  {
    print_centered(height / 2, current_player->playerName);
  }

  print_centered(height / 4, convertEnum(_game_model->current_phase->get()));

  WindowView::display();
}
// do phase after.
void StatisticsObserverView::display()
{
  wclear(_window);
  box(_window, 0, 0);

  std::vector<Player *> players = _game_model->active_players->get();

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

    std::vector<Player *> sortedPlayers;

    sortedPlayers.assign(players.begin(), players.end());

    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](auto playerA, auto playerB) {
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

      if (percent_owned > 0.05)
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

      std::ostringstream convertPercentage;

      convertPercentage << std::fixed;
      convertPercentage << std::setprecision(2);

      convertPercentage << percent_owned * 100;

      std::string percent_ownedString = " " + convertPercentage.str() + "%%";
      std::string number_of_territories_ownedString = " (" + std::to_string(number_of_territories) + " territories)";
      
      std::string displayTerritoryStringInfo =  percent_ownedString + number_of_territories_ownedString;
      
      wprintw(_window, displayTerritoryStringInfo.c_str());
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
  _game_model->current_player->detach(this);
  _game_model->current_phase->detach(this);
}

StatisticsObserverView::~StatisticsObserverView()
{
  _game_model->active_players->detach(this);
}

GameplayView::GameplayView(int w, int h, GameModel *sm)
{
  settings_model = sm;
  settings_model->log->attach(this);
  settings_model->current_step->attach(this);
  settings_model->current_order_type->attach(this);
  settings_model->territory_list_items->attach(this);
  settings_model->selected_index->attach(this);
  settings_model->error_message->attach(this);
  bool headers_enabled = settings_model->phase_headers_enabled->get() || settings_model->stats_headers_enabled->get();
  this->start_x = 1;
  this->start_y = headers_enabled * LINES / 4;
  this->height = h - start_y;
  this->width = w;
}

GameplayView::~GameplayView()
{
  settings_model->log->detach(this);
  settings_model->current_step->detach(this);
  settings_model->current_order_type->detach(this);
  settings_model->territory_list_items->detach(this);
  settings_model->selected_index->detach(this);
  settings_model->error_message->detach(this);

  if (_phase_view)
    delete _phase_view;
  if (_stats_view)
    delete _stats_view;
}

void GameplayView::update()
{
  display();
}

void GameplayView::create_phase_observer_view(int header_height)
{
  _phase_view = new PhaseObserverView(COLS / 2 - 1, header_height, 1, 1, settings_model);
}

void GameplayView::create_stats_observer_view(int header_height)
{
  _stats_view = new StatisticsObserverView(COLS / 2 - 1, header_height, COLS / 2, 1, settings_model);
}

void GameplayView::display()
{
  if (!_window)
  {
    activate();
  }
  if (_phase_view)
  {
    _phase_view->display();
  }
  if (_stats_view)
  {
    _stats_view->display();
  }

  wclear(_window);

  if (settings_model->current_phase->get() == ISSUE_ORDERS && settings_model->current_step->get() >= 0) {
    // Draw human player UI
    int step = settings_model->current_step->get();
    if (step == 0)
        display_human_input_order_choice_step();
    else if (step == 1)
        display_human_source_input_step();
    else if (step == 2)
      display_human_target_input_step();

    if (settings_model->error_message->get() != "") {
      wattron(_window, COLOR_PAIR(BLACK_RED));
      print_centered(height / 2, settings_model->error_message->get());
      wattroff(_window, COLOR_PAIR(BLACK_RED));
    }
  } else {
    int index = 0;
    for (std::string msg : settings_model->log->get())
    {
      if (index > height - 3)
      {
        break;
      }

      int cp;
      if (index == 0)
      {
        cp = COLOR_PAIR(WHITE_BLACK);
      }
      else
      {
        cp = COLOR_PAIR(GREY_BLACK);
      }
      wattron(_window, cp);
      wmove(_window, 1 + index++, 1);
      wprintw(_window, msg.c_str());
      wattroff(_window, cp);
    }
  }

  box(_window, 0, 0);
  WindowView::display();
}

void GameplayView::display_human_source_input_step() {
  OrderType current_order_type = settings_model->current_order_type->get();
  std::stringstream instructions_stream;
  std::string header;

  if (current_order_type == DEPLOY)
  {
    header = "Deploy";
    instructions_stream << "Choose how many armies to deploy to each of your territories" << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Use LEFT/RIGHT arrows to adjust number of armies" << std::endl
                        << "Use 'd' to deploy all remaining armies to the selected territory" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ true);
  }
  else if (current_order_type == ADVANCE) {
    header = "Advance (Source)";
    instructions_stream << "Choose a source territory to advance from." << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Use LEFT/RIGHT arrows to adjust number of armies" << std::endl
                        << "Press BACKSPACE to return to previous menu" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ false, /* set_num_armies_to_move */ true);
  }
  else if (current_order_type == AIRLIFT) {
    header = "Airlift (Source)";
    instructions_stream << "Choose a source territory to airlift from" << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Use LEFT/RIGHT arrows to adjust number of armies" << std::endl
                        << "Press BACKSPACE to return to previous menu" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ false, /* set_num_armies_to_move */ true);
  }
  else if (current_order_type == BOMB)
  {
    header = "Bomb";
    instructions_stream << "Choose an enemy territory to bomb." << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Press BACKSPACE to return to previous menu" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ false, /* set_num_armies_to_move */ false);
  }
  else if (current_order_type == BLOCKADE)
  {
    header = "Blockade";
    instructions_stream << "Choose an enemy territory to blockade." << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Press BACKSPACE to return to previous menu" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ false, /* set_num_armies_to_move */ false);
  }
  else if (current_order_type == NEGOTIATE)
  {
    header = "Negotiate";
    instructions_stream << "Choose an enemy to negotiate with." << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Press BACKSPACE to return to previous menu" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ false, /* set_num_armies_to_move */ false);
  }
}

void GameplayView::display_human_target_input_step()
{
  OrderType current_order_type = settings_model->current_order_type->get();
  std::stringstream instructions_stream;
  std::string header;

  if (current_order_type == ADVANCE)
  {
    header = "Advance (Target)";
    instructions_stream << "Choose a advance territory to advance to." << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Use LEFT/RIGHT arrows to adjust number of armies" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ false, /* set_num_armies_to_move */ false);
  }
  else if (current_order_type == AIRLIFT)
  {
    header = "Airlift (Target)";
    instructions_stream << "Choose a target territory to airlift to" << std::endl
                        << std::endl
                        << "Use UP/DOWN arrows to select a territory" << std::endl
                        << "Use LEFT/RIGHT arrows to adjust number of armies" << std::endl;

    display_territory_select_menu(header, instructions_stream, /* deployments */ false, /* set_num_armies_to_move */ false);
  }
}

void GameplayView::display_territory_select_menu(std::string &left_header, std::stringstream &instructions_stream, bool deployments, bool set_num_armies_to_move)
{
  /* DISPLAY INSTRUCTIONS */
  Player *current_player = settings_model->current_player->get();
  std::string header = current_player->playerName + ": " + left_header;

  print_centered_at_col(6, width / 4, header);
  int off_y = 0;
  int max_row_width = width / 3 - 4;
  char next_line[max_row_width];

  int lines_read = 0;
  while (!instructions_stream.eof())
  {
    instructions_stream.getline(next_line, max_row_width);
    lines_read++;
    if (lines_read > height) break;

    print_centered_at_col(8 + off_y++, width / 4, next_line);
  }

  /* DISPLAY UI */
  int armies_remaining = current_player->getArmees();

  const std::vector<ConcreteObservable<std::pair<map::Territory *, int>> *> &territory_list_items = settings_model->territory_list_items->get();

  int index = 0;
  int current_index = settings_model->selected_index->get();
  int num_visible_rows = height - 9;
  int offset = (1 + current_index) - num_visible_rows;
  if (offset <= 0)
    offset = 0;

  if (offset > 0)
  {
    wmove(_window, 4, width / 2 + 1);
    wattron(_window, COLOR_PAIR(GREY_BLACK));
    wprintw(_window, (std::to_string(offset) + " more...").c_str());
    wattroff(_window, COLOR_PAIR(GREY_BLACK));
  }
  if (territory_list_items.size() - offset > num_visible_rows)
  {
    wmove(_window, height - 4, width / 2 + 1);
    wattron(_window, COLOR_PAIR(GREY_BLACK));
    wprintw(_window, (std::to_string(territory_list_items.size() - offset - num_visible_rows) + " more...").c_str());
    wattroff(_window, COLOR_PAIR(GREY_BLACK));
  }
  for (auto obs : territory_list_items)
  {
    auto pair = obs->get();
    map::Territory *territory = pair.first;

    if (deployments) {
      armies_remaining -= pair.second;
    } else if (index == current_index) {
      armies_remaining = pair.second;
    }

    if (index >= offset && index - offset < num_visible_rows)
    {
      int line = 5 + index - offset;
      if (index == current_index)
        wattron(_window, A_STANDOUT);
      else if (territory->getOwner() != current_player) {
        wattron(_window, COLOR_PAIR(RED_BLACK));
      }
      wmove(_window, line, width / 2 + 1);
      wprintw(_window, (territory->getName() + " (" + territory->getContinent()->getName() + ")").c_str());
      while (getcurx(_window) < width - 2)
        waddch(_window, ' ');

      std::string owner_name = "Neutral";
      if (territory->getOwner() != nullptr)
        owner_name = territory->getOwner()->playerName;

      if (deployments || set_num_armies_to_move)
        {
          print_right_aligned_at_col(
              line,
              width - 2,
              "+" + std::to_string(pair.second) + " (" + std::to_string(territory->getArmees()) + ")" + " - " + owner_name);
        }
      else
      {
        print_right_aligned_at_col(
            line,
            width - 2,
            "(" + std::to_string(territory->getArmees()) + ")" + " - " + owner_name);
      }
      wattroff(_window, A_STANDOUT);
      wattroff(_window, COLOR_PAIR(RED_BLACK));
    }

    index++;
  }

  if (armies_remaining == 0 || !deployments)
  {
    print_centered_at_col(height - 3, 3 * width / 4, "Press SPACE to Continue");
  }
  if (deployments) {
    print_centered_at_col(2, 3 * width / 4, std::to_string(armies_remaining) + " Armies Remaining");
  }
  else if (set_num_armies_to_move)
  {
    print_centered_at_col(2, 3 * width / 4, "Move " + std::to_string(armies_remaining) + " Armies");
  }
}

void GameplayView::display_human_input_order_choice_step() {
  print_centered(2, settings_model->current_player->get()->playerName);

  int row = 4;
  wattron(_window, COLOR_PAIR(GREY_BLACK));
  print_centered(row++, "Choose an order type to execute:");
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  int must_deploy = 0;
  int blockade_cards = settings_model->current_player->get()->countCardsOfType("blockade");
  int airlift_cards = settings_model->current_player->get()->countCardsOfType("airlift");
  int bomb_cards = settings_model->current_player->get()->countCardsOfType("bomb");
  int negotiate_cards = settings_model->current_player->get()->countCardsOfType("negotiate");

  wmove(_window, row + 1, width / 2 - 20);
  if (!must_deploy)
    wattron(_window, COLOR_PAIR(GREY_BLACK));
  wprintw(_window, "D. Deploy (%i)", must_deploy);
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  wmove(_window, row + 3, width / 2 - 20);
  if (must_deploy)
    wattron(_window, COLOR_PAIR(GREY_BLACK));
  wprintw(_window, "A. Advance (inf)");
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  wmove(_window, row + 5, width / 2 - 20);
  if (!blockade_cards || must_deploy)
    wattron(_window, COLOR_PAIR(GREY_BLACK));
  wprintw(_window, "S. Blockade (%i)", blockade_cards);
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  wmove(_window, row + 1, width / 2 + 5);
  if (!airlift_cards || must_deploy)
    wattron(_window, COLOR_PAIR(GREY_BLACK));
  wprintw(_window, "F. Airlift (%i)", airlift_cards);
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  wmove(_window, row + 3, width / 2 + 5);
  if (!bomb_cards || must_deploy)
    wattron(_window, COLOR_PAIR(GREY_BLACK));
  wprintw(_window, "B. Bomb (%i)", bomb_cards);
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  wmove(_window, row + 5, width / 2 + 5);
  if (!negotiate_cards || must_deploy)
    wattron(_window, COLOR_PAIR(GREY_BLACK));
  wprintw(_window, "N. Negotiate (%i)", negotiate_cards);
  wattroff(_window, COLOR_PAIR(GREY_BLACK));

  print_centered(row + 7, "Press SPACE to pass");
}

void GameplayView::activate()
{
  bool headers_enabled = settings_model->phase_headers_enabled->get() || settings_model->stats_headers_enabled->get();
  int header_height = settings_model->number_of_players->get() * 2 + 1;
  this->start_y = 1 + headers_enabled * header_height;
  this->height = LINES - start_y;

  if (_window && _window != stdscr)
  {
    deactivate();
  }
  _window = create_newwin(height, width, start_y, start_x);
  box(_window, 0, 0);
  display();

  if (settings_model->phase_headers_enabled->get())
  {
    create_phase_observer_view(header_height);
    _phase_view->activate();
  }

  if (settings_model->stats_headers_enabled->get())
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
}

void GameplayController::startupPhase()
{
  _game_model->current_phase->set(STARTUP);

  int num_players = _game_model->number_of_players->get();

  std::vector<Player *> newly_created_players;

  int armies_per_player = 50 - 5 * num_players;

  for (int i = 0; i < num_players; i++)
  {
    Player *new_player = new Player("Player " + std::to_string(i + 1), i);
    if (i == 0) {
      new_player->setStrategy(new HumanPlayerStrategy());
    } else {
      new_player->setStrategy(new NeutralPlayerStrategy());
    }

    // sets the starting armies for each player accoridng to the number of players playing the game
    new_player->setArmees(armies_per_player);

    newly_created_players.push_back(new_player);
  }

  // adding initial armys

  while (!newly_created_players.empty())
  {
    int index = rand() % newly_created_players.size();

    Player *next = newly_created_players[index];
    _game_model->active_players->push_back(next);

    newly_created_players.erase(newly_created_players.begin() + index);
  }

  Player *first = _game_model->active_players->get()[0];
  _game_model->current_player->set(first);

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
    Player *player = _game_model->active_players->get()[index_of_next_player_to_receive_territory];
    territory->setOwner(player);
    _game_model->log->append("Assigned " + territory->getName() + " to " + player->playerName);

    territories.erase(territories.begin() + territory_index);

    index_of_next_player_to_receive_territory = ++index_of_next_player_to_receive_territory % _game_model->number_of_players->get();

#ifdef __linux__
    usleep(HIT_DA_BREAKS * 100);
#else
    Sleep(HIT_DA_BREAKS);
#endif
  }
}

void GameplayController::mainGameLoop() {
  if (_game_model->active_players->get().size() > 1) {
    _game_model->log->append("Do you wish to change strategies? (y/n)");
    if (Application::instance()->get_key(true) == 'y')
    {
      std::vector<Player *> players = _game_model->active_players->get();
      for (Player *player : players)
      {
        _game_model->log->append(player->playerName + " strategy: (h, b, a, n)");
        int key = Application::instance()->get_key(true);
        if (key == 'h')
        {
          player->setStrategy(new HumanPlayerStrategy());
        }
        else if (key == 'b')
        {
          player->setStrategy(new BenevolentPlayerStrategy());
        }
        else if (key == 'a')
        {
          // player->setStrategy(new AggressivePlayerStrategy());
        }
        else if (key == 'n')
        {
          player->setStrategy(new NeutralPlayerStrategy());
        }
      }
    }

    reinforcementPhase();
    issueOrdersPhase();
    executeOrdersPhase();
  }
}

void GameplayController::removeDeadPlayers() {
  std::list<Player*> players_to_remove;
  for (Player* player : _game_model->active_players->get()) {
    if (player->owned_territories.empty()) players_to_remove.push_back(player);
    // else if (rand() % 10 == 0) { // Purposefully commented. Uncomment to unsure game will eventually complete
    //    players_to_remove.push_back(player);
    // }
  }

  for (Player* player : players_to_remove) {
    _game_model->log->append(player->playerName + " has lost all territories and is no longer in the game.");
    _game_model->active_players->remove(player);
  }

  if (_game_model->active_players->get().size() == 1) {
    _game_model->log->append("Game Over. " + _game_model->active_players->get()[0]->playerName + " has won!");
    _game_model->log->append("Press BACKSPACE to return to Main Menu.");
  }
}

void GameplayController::reinforcementPhase() {
  _game_model->current_phase->set(REINFORCEMENT);
  _game_model->log->append("Reinforcement phase started");
  
  //Go through players
  for (auto player : _game_model->active_players->get())
  {
    //Find total territory count for each player
    int ownedTerritories = player->owned_territories.size();
    int armiesToAssign = (int)floor(ownedTerritories / 3);
    int bonus = getPlayersBonus(player);
    //assign continent bonus
     _game_model->log->append("Initial armies: " + std::to_string(player->getArmees()));


    if(bonus != 0)
       _game_model->log->append("Bonus received: " + std::to_string(bonus));

    armiesToAssign += getPlayersBonus(player);

    //Minimum 3 armies if no armies
    if(armiesToAssign <= 3){
      armiesToAssign = 3;
    }

    _game_model->log->append("Armies assigned in reinforcement phase: " + std::to_string(armiesToAssign));
    
    // Add assignArmies to player
    player->setArmees(player->getArmees() + armiesToAssign);

    _game_model->log->append("Total armies for player: " + player->playerName + " " + std::to_string(player->getArmees()));
  }
}

void GameplayController::issueOrdersPhase() {
  _game_model->current_phase->set(ISSUE_ORDERS);

  /* Copy all active players into a pool of players waiting to issue orders */
  std::vector<Player*> active_players = _game_model->active_players->get();

  std::vector<Player*> players_wanting_to_issue_orders;
  players_wanting_to_issue_orders.assign(active_players.begin(), active_players.end());
  
  int index_of_current_player = 0;
  while (!players_wanting_to_issue_orders.empty()) {
    index_of_current_player %= players_wanting_to_issue_orders.size();

    Player *current = players_wanting_to_issue_orders[index_of_current_player];
    _game_model->current_player->set(current);

    // Allow the current player to issue 1 order, or return nullptr indicating they are finished
    Order* issued = current->issueOrder(_game_model);

    if (issued != nullptr) {
      _game_model->log->append(current->playerName + " issued: " + issued->toString());
      // Go to next player who still wants to issue an order.
    } else {
      // If no order was issued, that means the Player is done issuing orders, and should be removed from the pool
      _game_model->log->append(current->playerName + " has finished issuing orders for this round");
      players_wanting_to_issue_orders.erase(players_wanting_to_issue_orders.begin() + index_of_current_player);
    }

    index_of_current_player++;

#ifdef __linux__
    usleep(HIT_DA_BREAKS * 300);
#else
    Sleep(HIT_DA_BREAKS * 3);
#endif
  }
}

void GameplayController::executeOrdersPhase() {
  _game_model->current_phase->set(ORDERS_EXECUTION);

  /* Copy all active players into a pool of players with orders still left to execute */
  std::vector<Player *> active_players = _game_model->active_players->get();

  std::vector<Player *> players_with_orders_to_execute;
  players_with_orders_to_execute.assign(active_players.begin(), active_players.end());

  int index_of_current_player = 0;
  while (!players_with_orders_to_execute.empty()) 
  {
    index_of_current_player %= players_with_orders_to_execute.size();

    Player *current = players_with_orders_to_execute.at(index_of_current_player);
    _game_model->current_player->set(current);

    // Request next order to be executed
    Order *order_to_execute = current->nextOrder();
    if (order_to_execute != nullptr)
    {
      order_to_execute->execute();
      _game_model->log->append(current->playerName + " executed " + order_to_execute->toString() + ": " + order_to_execute->getEffect());
    }
    else
    {
      // If no order was returned, that means the Player has no more orders to execute
      _game_model->log->append(current->playerName + " is done executing orders.");
      players_with_orders_to_execute.erase(players_with_orders_to_execute.begin() + index_of_current_player);
    }

    index_of_current_player++;

    // Go to next player who still has orders to execute
#ifdef __linux__
    usleep(HIT_DA_BREAKS * 300);
#else
    Sleep(HIT_DA_BREAKS * 3);
#endif
  }
  removeDeadPlayers();
}

int GameplayController::getPlayersBonus(Player * p)
{
  int totalBonus = 0;
  bool ownsThisContinent = true;

  for (auto continent : _game_model->map->getContinents())
  {
    for (auto territory : _game_model->map->getTerritories())
    {
      if (territory->getOwner() != p)
      {
        ownsThisContinent = false;
        break;
        //Check next continent
      }
    }
    //They own the continent
    if (ownsThisContinent)
    {
      totalBonus += continent->getBonus();
    }
  }

  return totalBonus;
}

bool GameplayController::keyboardEventPerformed(int key) {
  if (key == KEY_BACKSPACE) {
    Application::instance()->activateView(MAIN_MENU_VIEW);
    return true;
  }
  mainGameLoop();
  return false;
}

void GameplayController::viewDeactivated()
{
  delete _game_model->map;
  _game_model->map = nullptr;

  for (auto player : _game_model->active_players->get())
  {
    _game_model->active_players->remove(player);
    delete player;
  }

  _game_model->current_player->set(nullptr);
}