#include "GameEngine.h"


MainMenuView::MainMenuView(int w, int h, SettingsModel *mgm) : WindowView(w, h, (COLS - w) / 2, (LINES - h) / 2)
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

MainMenuController::MainMenuController(SettingsModel *mgm)
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
  else if (key == KEY_UP)
  {
    int nNumberOfPlayers = _settings_model->number_of_players.get() + 1;
    if (nNumberOfPlayers > 5)
      nNumberOfPlayers = 5;
    _settings_model->number_of_players.set(nNumberOfPlayers);
  }
  else if (key == KEY_DOWN)
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


void MapSelectionView::registerMenuListener(ActionListener *listener) {
  _menu_view->registerListener(listener);
}


void MapSelectionView::activate() {
  WindowView::activate();
  _menu_view->activate();
}


void MapSelectionView::deactivate()
{
  WindowView::deactivate();
  _menu_view->deactivate();
}


void MapSelectionView::display() {
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
  if (key == KEY_UP)
  {
    _menu_model->error_message.set("");
    _menu_model->incrementItem(-1);
  }
  else if (key == KEY_DOWN)
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

MapSelectionController::MapSelectionController(MenuModel *mm)
{
  _menu_model = mm;
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

    map::Map map;
    MapLoader mapLoader;

    if (mapLoader.loadFile(map_file, map) && map.validate())
    {
      // load file under
      Application::instance()->activateView(GAMEPLAY_VIEW);
    } else {
      _menu_model->error_message.set("This is a invalid file! Please choose another!");
    }

    return true;
  }
  return false;
}
void MapSelectionController::viewActivated() {
  map::Map map;
  MapLoader maploader;

  std::vector<std::string> map_list = maploader.findMapFiles();
  _menu_model->map_file_list.set(map_list);
}
void MapSelectionController::viewDeactivated() {}

PhaseObserverView::PhaseObserverView(int w, int h, int x, int y) : WindowView(w, h, x, y) {}
StatisticsObserverView::StatisticsObserverView(int w, int h, int x, int y) : WindowView(w, h, x, y) {}

void PhaseObserverView::display() {
  wclear(_window);
  box(_window, 0, 0);
  print_centered(height / 2, "Phase info");
  WindowView::display();
}

void StatisticsObserverView::display() {
  wclear(_window);
  box(_window, 0, 0);
  print_centered(height / 2, "Stats info");
  WindowView::display();
}

GameplayView::GameplayView(int w, int h, SettingsModel *sm) {
  settings_model = sm;
  bool headers_enabled = settings_model->phase_headers_enabled.get() || settings_model->stats_headers_enabled.get();
  this->start_x = 1;
  this->start_y = headers_enabled * LINES / 4;
  this->height = h - start_y;
  this->width = w;
}

GameplayView::~GameplayView() {
  if (_phase_view)
    delete _phase_view;
  if (_stats_view)
    delete _stats_view;
}

void GameplayView::create_phase_observer_view() {
  _phase_view = new PhaseObserverView(COLS / 2 - 1, LINES / 4 - 1, 1, 1);
}

void GameplayView::create_stats_observer_view()
{
  _stats_view = new StatisticsObserverView(COLS / 2 - 1, LINES / 4 - 1, COLS / 2, 1);
}

void GameplayView::display() {
  int header_offset = 0;
  if (_phase_view) {
    _phase_view->display();
  }
  if (_stats_view) {
    _stats_view->display();
  }
  if (!_window) {
    activate();
  }
  wclear(_window);
  box(_window, 0, 0);

  print_centered(height / 2, "MAIN CONTENT");

  WindowView::display();
}

void GameplayView::activate() {
  bool headers_enabled = settings_model->phase_headers_enabled.get() || settings_model->stats_headers_enabled.get();
  this->start_y = 1 + headers_enabled * LINES / 4;
  this->height = LINES - start_y;

  if (settings_model->phase_headers_enabled.get()) {
    create_phase_observer_view();
    _phase_view->activate();
  }
    
  if (settings_model->stats_headers_enabled.get()) {
    create_stats_observer_view();
    _stats_view->activate();
  }

  WindowView::activate();
  box(_window, 0, 0);
  wrefresh(_window);
}

void GameplayView::deactivate() {
  WindowView::deactivate();
  if (_phase_view) {
    _phase_view->deactivate();
  }
  if (_stats_view)
  {
    _stats_view->deactivate();
  }
}

