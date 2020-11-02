#include "GameEngine.h"


MainMenuView::MainMenuView(int w, int h, GameModel* mgm): WindowView(w, h, (COLS - w) / 2, (LINES - h) / 2) {
  _game_model = mgm;
  _game_model->getPhaseHeadersEnabled()->attach(this);
  _game_model->getStatsHeadersEnabled()->attach(this);
};

MainMenuView::~MainMenuView() {
  _game_model->getPhaseHeadersEnabled()->detach(this);
  _game_model->getStatsHeadersEnabled()->detach(this);
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
  print_centered(offset + 3, "Press SPACE to begin");
  wattroff(_window, COLOR_PAIR(RED_BLACK));


  if (_game_model->getPhaseHeadersEnabled()->get()) {
    wattron(_window, COLOR_PAIR(BLACK_GREEN));
    print_centered(offset + 5, " PHASE HEADER ENABLED  (p) ");
    wattroff(_window, COLOR_PAIR(BLACK_GREEN));
  } else {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(offset + 5, " PHASE HEADER DISABLED (p) ");
    wattroff(_window, COLOR_PAIR(BLACK_RED));
  }

  if (_game_model->getStatsHeadersEnabled()->get())
  {
    wattron(_window, COLOR_PAIR(BLACK_GREEN));
    print_centered(offset + 7, " STATS HEADER ENABLED  (o) ");
    wattroff(_window, COLOR_PAIR(BLACK_GREEN));
  }
  else
  {
    wattron(_window, COLOR_PAIR(BLACK_RED));
    print_centered(offset + 7, " STATS HEADER DISABLED (o) ");
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

ConcreteObservable<bool> *GameModel::getPhaseHeadersEnabled()
{
  return &phase_headers_enabled;
}

ConcreteObservable<bool> *GameModel::getStatsHeadersEnabled()
{
  return &stats_headers_enabled;
}

void GameModel::setPhaseHeadersEnabled(bool e)
{
  phase_headers_enabled.set(e);
}

void GameModel::setStatsHeadersEnabled(bool e)
{
  stats_headers_enabled.set(e);
}

GameController::GameController(GameModel * mgm)
{
  _game_model = mgm;
}

GameController::~GameController() {
}

bool GameController::keyboardEventPerformed(int key) {
  if (key == 'p') {
    bool current = _game_model->getPhaseHeadersEnabled()->get();
    _game_model->setPhaseHeadersEnabled(!current);
    return true;
  } else if (key == 'o') {
    bool current = _game_model->getStatsHeadersEnabled()->get();
    _game_model->setStatsHeadersEnabled(!current);
    return true;
  } else if (key == ' ') {
    Application::instance()->activateView(1);
    return true;
  }
  return false;
}