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

  SettingsModel *game_model = new SettingsModel();
  game_model->phase_headers_enabled.set(true);
  game_model->number_of_players.set(2);

  MenuModel *menu_model = new MenuModel();


/************************************************* MAP LOADER PARTS ADDED by brenden */

  // adding the maps
    map::Map map;
    MapLoader maploader;

/* *********************************************************************************** */

  std::vector<std::string> map_list = maploader.findMapFiles();
  menu_model->map_file_list.set(map_list);

  MainMenuController *main_game_controller = new MainMenuController(game_model);
  MapSelectionController *map_selection_controller = new MapSelectionController(menu_model);
  MapMenuController *map_menu_controller = new MapMenuController(menu_model);

  MainMenuView *main_menu_view = new MainMenuView(54, LINES, game_model);
  MapSelectionView *map_selection_view = new MapSelectionView(54, LINES, 6, menu_model);

  main_menu_view->registerListener(main_game_controller);

  map_selection_view->registerListener(map_selection_controller);
  map_selection_view->registerMenuListener(map_menu_controller);

  std::shared_ptr<Application> application = Application::instance();
  application->registerView(MAIN_MENU_VIEW, main_menu_view);
  application->registerView(MAP_SELECTION_VIEW, map_selection_view);
  application->activateView(MAIN_MENU_VIEW);

  application->mainloop('q');
  clear();
  refresh();

  delete map_selection_view;
  delete main_menu_view;
  delete main_game_controller;
  delete map_selection_controller;
  delete map_menu_controller;
  delete game_model;
  delete menu_model;

  endwin();
  return 0;
}
