#include "GameEngine.h"

View::View() {
}

View::~View() {
}

void View::registerListener(ActionListener* action_listener) {
  _action_listeners.push_back(action_listener);
}

void View::deregisterListener(ActionListener *action_listener) {
  _action_listeners.remove(action_listener);
}

void View::notifyKeyboardEventPerformed(int key) {
  for (auto listener : _action_listeners) {
    if (listener->keyboardEventPerformed(key)) break;
  }
}

void View::display() {}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
  // Implementation based on https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);

  return local_win;
}

WindowView::WindowView() {
  _window = stdscr;
  start_x, start_y = 0;
  getmaxyx(stdscr, height, width);
}

WindowView::WindowView(int w, int h, int x, int y) {
  width = w;
  height = h;
  start_x = x;
  start_y = y;
  _window = create_newwin(h, w, y, x);
}

WindowView::~WindowView()
{
  deactivate();
}

void WindowView::deactivate() {
  if (_window && _window != stdscr)
  {
    wclear(_window);
    wrefresh(_window);
    delwin(_window);
    _window = nullptr;
  }
}

void WindowView::activate() {
  if (_window && _window != stdscr) {
    deactivate();
  }
  _window = create_newwin(height, width, start_y, start_x);
  display();
}

void WindowView::mainloop(char esc_key) {  
  curs_set(0);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  std::stringstream escape_msg;
  escape_msg << "Press " << esc_key << " to exit";
  printw((escape_msg).str().c_str());
  refresh();

  display();

  int ch;
  while ((ch = getch()) != esc_key)
  {
    notifyKeyboardEventPerformed(ch);
  }
}

void WindowView::display() {
  wrefresh(_window);
}

void WindowView::print_centered(int line, std::string msg) {
  int offset = (width - msg.length()) / 2;
  if (offset < 0) offset = 0;
  wmove(_window, line, offset);
  wprintw(_window, msg.c_str());
}

MainMenuView::MainMenuView(int w, int h, MainGameModel* mgm): WindowView(w, h, (COLS - w) / 2, (LINES - h) / 2) {
  _main_game_model = mgm;
  _main_game_model->getPhaseHeadersEnabled()->attach(this);
  _main_game_model->getStatsHeadersEnabled()->attach(this);
};

MainMenuView::~MainMenuView() {
  _main_game_model->getPhaseHeadersEnabled()->detach(this);
  _main_game_model->getStatsHeadersEnabled()->detach(this);
}

void MainMenuView::display_banner(int& offset) {
  wattron(_window, COLOR_PAIR(RED_BLACK));
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
  print_centered(offset + 3, "Created by");
  wattron(_window, COLOR_PAIR(WHITE_BLACK));
  print_centered(offset + 4, "Alex Kofman    Anthony van Voorst    Brendan Bissessar");
  print_centered(offset + 5, "Drew Wagner    Jonathan Andrei");
  offset = getcury(_window);
  wattroff(_window, COLOR_PAIR(WHITE_BLACK));
}

void MainMenuView::display_menu(int& offset) {
  if (_main_game_model->getPhaseHeadersEnabled()->get()) {
    wattron(_window, COLOR_PAIR(BLACK_GREEN));
    print_centered(offset + 3, "PHASE HEADERS ENABLED  (p)");
    wattroff(_window, COLOR_PAIR(BLACK_GREEN));
  } else {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(offset + 3, "PHASE HEADERS DISABLED (p)");
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }

  if (_main_game_model->getStatsHeadersEnabled()->get())
  {
    wattron(_window, COLOR_PAIR(BLACK_GREEN));
    print_centered(offset + 5, "STATS HEADERS ENABLED  (o)");
    wattroff(_window, COLOR_PAIR(BLACK_GREEN));
  }
  else
  {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(offset + 5, "STATS HEADERS DISABLED (o)");
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }
  offset = getcury(_window);
}

void MainMenuView::display() {
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

ConcreteObservable<int>* MainGameModel::getPhase() {
  return &phase;
}

ConcreteObservable<bool> *MainGameModel::getPhaseHeadersEnabled()
{
  return &phase_headers_enabled;
}

ConcreteObservable<bool> *MainGameModel::getStatsHeadersEnabled()
{
  return &stats_headers_enabled;
}

void MainGameModel::setPhase(int p) {
  phase.set(p);
}

void MainGameModel::setPhaseHeadersEnabled(bool e)
{
  phase_headers_enabled.set(e);
}

void MainGameModel::setStatsHeadersEnabled(bool e)
{
  stats_headers_enabled.set(e);
}

MainGameController::MainGameController(MainGameModel * mgm)
{
  _main_game_model = mgm;
}

MainGameController::~MainGameController() {
}

bool MainGameController::keyboardEventPerformed(int key) {
 if (key == 'p') {
   bool current = _main_game_model->getPhaseHeadersEnabled()->get();
   _main_game_model->setPhaseHeadersEnabled(!current);
   return true;
 } else if (key == 'o') {
   bool current = _main_game_model->getStatsHeadersEnabled()->get();
   _main_game_model->setStatsHeadersEnabled(!current);
   return true;
 }
 return false;
}