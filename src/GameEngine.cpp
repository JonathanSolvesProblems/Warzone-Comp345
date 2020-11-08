#include "GameEngine.h"


MainMenuView::MainMenuView(int w, int h, SettingsModel* mgm): WindowView(w, h, (COLS - w) / 2, (LINES - h) / 2) {
  _settings_model = mgm;
  _settings_model->getPhaseHeadersEnabled()->attach(this);
  _settings_model->getStatsHeadersEnabled()->attach(this);
  _settings_model->getNumberOfPlayers()->attach(this);
};

MainMenuView::~MainMenuView() {
  _settings_model->getPhaseHeadersEnabled()->detach(this);
  _settings_model->getStatsHeadersEnabled()->detach(this);
  _settings_model->getNumberOfPlayers()->detach(this);
}

void MainMenuView::display_banner(int& offset) {
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

void MainMenuView::display_credits(int& offset) {
  wattron(_window, COLOR_PAIR(GREY_BLACK));
  print_centered(height - 4, "Created by");
  wattron(_window, COLOR_PAIR(WHITE_BLACK));
  print_centered(height - 3, "Alex Kofman    Anthony van Voorst    Brendan Bissessar");
  print_centered(height - 2, "Drew Wagner    Jonathan Andrei");
  wattroff(_window, COLOR_PAIR(WHITE_BLACK));
}

void MainMenuView::display_menu(int& offset) {
  wattron(_window, COLOR_PAIR(RED_BLACK));
  print_centered(offset + 3, "Number of Players (UP/DOWN):  ");
  wattron(_window, COLOR_PAIR(WHITE_BLACK));
  print_centered(offset + 5, std::to_string(_settings_model->getNumberOfPlayers()->get()).c_str());
  wattron(_window, COLOR_PAIR(RED_BLACK));
  print_centered(offset + 7, "Press SPACE to begin");
  wattroff(_window, COLOR_PAIR(RED_BLACK));
  
  if (_settings_model->getPhaseHeadersEnabled()->get()) {
    wattron(_window, COLOR_PAIR(BLACK_GREEN));
    print_centered(offset + 9, " PHASE HEADER ENABLED  (p) ");
    wattroff(_window, COLOR_PAIR(BLACK_GREEN));
  } else {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(offset + 9, " PHASE HEADER DISABLED (p) ");
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }

  if (_settings_model->getStatsHeadersEnabled()->get())
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

void MainMenuView::display() {
  if (!_window) {
    activate();
  }

  int offset = 0;
  wclear(_window);
  display_banner(offset);
  display_menu(offset);
  display_credits(offset);
  WindowView::display();
}

void MainMenuView::update() {
  display();
}

SettingsModel::SettingsModel() {
  setNumberOfPlayers(2);
}

ConcreteObservable<bool> *SettingsModel::getPhaseHeadersEnabled()
{
  return &phase_headers_enabled;
}

ConcreteObservable<bool> *SettingsModel::getStatsHeadersEnabled()
{
  return &stats_headers_enabled;
}

ConcreteObservable<int> *SettingsModel::getNumberOfPlayers()
{
  return &number_of_players;
}

void SettingsModel::setPhaseHeadersEnabled(bool e)
{
  phase_headers_enabled.set(e);
}

void SettingsModel::setStatsHeadersEnabled(bool e)
{
  stats_headers_enabled.set(e);
}

void SettingsModel::setNumberOfPlayers(int n)
{
  number_of_players.set(n);
}

MainMenuController::MainMenuController(SettingsModel * mgm)
{
  _settings_model = mgm;
}

MainMenuController::~MainMenuController() {
}

bool MainMenuController::keyboardEventPerformed(int key) {
  if (key == 'p') {
    bool current = _settings_model->getPhaseHeadersEnabled()->get();
    _settings_model->setPhaseHeadersEnabled(!current);
    return true;
  } else if (key == 'o') {
    bool current = _settings_model->getStatsHeadersEnabled()->get();
    _settings_model->setStatsHeadersEnabled(!current);
    return true;
  } else if (key == KEY_UP) {
    int nNumberOfPlayers = _settings_model->getNumberOfPlayers()->get() + 1;
    if (nNumberOfPlayers > 5) nNumberOfPlayers = 5;
    _settings_model->setNumberOfPlayers(nNumberOfPlayers);
  }
  else if (key == KEY_DOWN)
  {
    int nNumberOfPlayers = _settings_model->getNumberOfPlayers()->get() - 1;
    if (nNumberOfPlayers < 2)
      nNumberOfPlayers = 2;
    _settings_model->setNumberOfPlayers(nNumberOfPlayers);
  } else if (key == ' ')
  {
    Application::instance()->activateView(MAP_SELECTION_VIEW);
    return true;
  }
  return false;
}

MapMenuView::MapMenuView(int w, int h, MenuModel *mm): WindowView(w, h, (COLS - w) / 2, LINES - h) {
  _menu_model = mm;
  _menu_model->getMapFileList()->attach(this);
  _menu_model->getSelectedItem()->attach(this);
}

MapMenuView::~MapMenuView() {
  _menu_model->getMapFileList()->detach(this);
  _menu_model->getSelectedItem()->detach(this);
}

void MapMenuView::update() {
  display();
}

void MapMenuView::display() {
  if (!_window)
  {
    activate();
  }

  wclear(_window);
  wattron(_window, COLOR_PAIR(RED_BLACK));
  box(_window, 0, 0);
  wattroff(_window, COLOR_PAIR(RED_BLACK));
  std::vector<std::string> maps = _menu_model->getMapFileList()->get();
  int active_index = _menu_model->getSelectedItem()->get();
  for (int i=0; i < maps.size(); i++) {
    if (i == active_index) {
      wattron(_window, COLOR_PAIR(BLACK_RED));
      print_centered(1 + i*2, maps[i]);
      wattroff(_window, COLOR_PAIR(BLACK_RED));
    } else {
      print_centered(1 + i*2, maps[i]);
    }
  }

  WindowView::display();
}

MapSelectionView::MapSelectionView(int w, int h, int header_height, MenuModel *mm) : WindowView(w, header_height, (COLS - w) / 2, 0)
{
  _menu_view = new MapMenuView(w, h - header_height, mm);
}

MapSelectionView::~MapSelectionView() {
  delete _menu_view;
}

void MapSelectionView::notifyKeyboardEventPerformed(int key) {
  // Send everything except SPACE To the sub view
  if (key != ' ' && key != KEY_BACKSPACE) {
    _menu_view->notifyKeyboardEventPerformed(key);
  } else {
    View::notifyKeyboardEventPerformed(key);
  }
}

void MapSelectionView::registerMenuListener(ActionListener *listener) {
  _menu_view->registerListener(listener);
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

ConcreteObservable<std::vector<std::string>> *MenuModel::getMapFileList() {
  return &_map_file_list;
}

ConcreteObservable<int> *MenuModel::getSelectedItem() {
  return &_selected_item;
}

void MenuModel::setMapFileList(std::vector<std::string>& list) {
  _map_file_list.set(list);
}

void MenuModel::setSelectedItem(int index) {
  const int length = _map_file_list.get().size();
  _selected_item.set(index % length);
}

void MenuModel::incrementItem(int inc) {
  int current = _selected_item.get();
  setSelectedItem(current + inc);
}

std::string MenuModel::getSelection() {
  return _map_file_list.get()[_selected_item.get()];
}

MapMenuController::MapMenuController(MenuModel *mm) {
  _menu_model = mm;
}

MapMenuController::~MapMenuController() {}

bool MapMenuController::keyboardEventPerformed(int key) {
  if (key == KEY_UP) {
    _menu_model->incrementItem(-1);
  } else if (key == KEY_DOWN) {
    _menu_model->incrementItem(1);
  } else {
    return false;
  }
  return true;
}

MapSelectionController::MapSelectionController(MenuModel *mm) {
  _menu_model = mm;
}

MapSelectionController::~MapSelectionController() {}

bool MapSelectionController::keyboardEventPerformed(int key) {
  if (key == ' ') {
    // Load map file
    std::string map_file = _menu_model->getSelection();

    Application::instance()->activateView(MAIN_MENU_VIEW);

    return true;
  } else if (key == KEY_BACKSPACE) {
    Application::instance()->activateView(MAIN_MENU_VIEW);
  }
  return false;
}