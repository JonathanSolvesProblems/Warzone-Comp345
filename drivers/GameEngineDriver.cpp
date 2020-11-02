#include "GameEngine.h"

void initialize_ncurses_colors() {
  start_color();
  init_color(COLOR_GREY, 600, 600, 600);
  init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
  init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(GREY_BLACK, COLOR_GREY, COLOR_BLACK);
  init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);
  init_pair(BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
}

int main () {
  initscr();
  initialize_ncurses_colors();

  GameModel *game_model = new GameModel();
  game_model->setPhaseHeadersEnabled(true);

  GameController *main_game_controller = new GameController(game_model);
  MainMenuView *main_menu_view = new MainMenuView(54, LINES, game_model);
  main_menu_view->registerListener(main_game_controller);

  std::shared_ptr<Application> application = Application::instance();
  application->registerView(MAIN_MENU_VIEW, main_menu_view);
  application->activateView(MAIN_MENU_VIEW);

  application->mainloop('q');
  clear();
  refresh();

  delete main_menu_view;
  delete main_game_controller;
  delete game_model;

  endwin();
  return 0;
}
