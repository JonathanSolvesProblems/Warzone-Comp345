#include "GameEngine.h"

int main () {
  initscr();
  start_color();
  init_color(COLOR_GREY, 600, 600, 600);
  init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
  init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(GREY_BLACK, COLOR_GREY, COLOR_BLACK);
  init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);
  init_pair(BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);

  MainGameModel *main_game_model = new MainGameModel();
  main_game_model->setPhaseHeadersEnabled(true);

  MainGameController *main_game_controller = new MainGameController(main_game_model);
  MainMenuView *main_menu_view = new MainMenuView(54, 20, main_game_model);
  main_menu_view->registerListener(main_game_controller);
  main_menu_view->mainloop('q');

  delete main_menu_view;
  delete main_game_controller;
  delete main_game_model;

  endwin();
  return 0;
}
