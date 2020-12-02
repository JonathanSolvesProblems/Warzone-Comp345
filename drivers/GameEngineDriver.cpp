#include "GameEngine.h"
#include <exception>
#ifdef __linux__
#include <signal.h>
#include <stdio.h>
#endif
static GameModel *game_model{nullptr};
static MenuModel *menu_model{nullptr};

static MainMenuController *main_game_controller{nullptr};
static MapSelectionController *map_selection_controller{nullptr};
static MapMenuController *map_menu_controller{nullptr};
static GameplayController *gameplay_controller{nullptr};

static MainMenuView *main_menu_view{nullptr};
static MapSelectionView *map_selection_view{nullptr};
static GameplayView *gameplay_view{nullptr};

/* Initializes colors and color pairs to be used with NCurses */
void initialize_ncurses_colors()
{
  start_color();
  /*         ID          RED  GREEEN  BLUE        */
  init_color(COLOR_GREY, 600, 600, 600);

  /*           ID         TEXT    BACKGROUND      */
  init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
  init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(GREY_BLACK, COLOR_GREY, COLOR_BLACK);
  init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);
  init_pair(BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
}

void cleanup() {
  /* Clear the screen before exiting, so the the terminal will function correctly afterwards */
  clear();
  refresh();

  /* Delete pointers */
  delete map_selection_view;
  delete main_menu_view;
  delete gameplay_view;

  delete gameplay_controller;
  delete main_game_controller;
  delete map_selection_controller;
  delete map_menu_controller;

  delete game_model;
  delete menu_model;

  /* Terminate Ncurses mode */
  endwin();
}

void signal_handler(int s) {
  cleanup();
  std::cerr << "Interupt caught: cleaning up  before exiting..." << std::endl;
  exit(1);
}

/*
 * A NOTE REGARDING MEMORY LEAKS
 * 
 * The NCurses library does not free all memory, and results in memory leaks under normal operation.
 * See the following link for more information:
 * 
 * https://invisible-island.net/ncurses/ncurses.faq.html#config_leaks
 * 
 * Using the Valgrind tool, we checked the memory leaks for GameEngineDriver.cpp and found that
 * they all originated from NCurses and is a normal part of the libaries internal functioning.
 * You can confirm this by commenting out all code between initscr() and endwin(), and you will
 * see that the memory leaked is approximately the same as when our program runs. The difference
 * is due to the way that NCurses operates when drawing to the screen.
 */
int main()
{

#ifdef __linux__
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = signal_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
#endif

  srand(time(NULL));

  /* Enter Ncurses mode */
  initscr();
  initialize_ncurses_colors();

  /* Declare game model and initialize some defaults */
  game_model = new GameModel();
  game_model->phase_headers_enabled->set(true);
  game_model->stats_headers_enabled->set(true);
  game_model->number_of_players->set(2);

  /* Menu model for selecting Map files */
  menu_model = new MenuModel();

  /* Create controllers */
  main_game_controller = new MainMenuController(game_model);
  map_selection_controller = new MapSelectionController(menu_model, game_model);
  map_menu_controller = new MapMenuController(menu_model);
  gameplay_controller = new GameplayController(game_model);

  /* Create Views */
  main_menu_view = new MainMenuView(54, LINES, game_model);
  map_selection_view = new MapSelectionView(54, LINES, 6, menu_model);
  gameplay_view = new GameplayView(COLS - 2, LINES - 2, game_model);

  /* Register Controllers as listeners to the corresponding View */
  main_menu_view->registerListener(main_game_controller);

  map_selection_view->registerListener(map_selection_controller);
  map_selection_view->registerMenuListener(map_menu_controller);

  gameplay_view->registerListener(gameplay_controller);

  /* Register views to the Application singleton */
  std::shared_ptr<Application> application = Application::instance();
  application->registerView(MAIN_MENU_VIEW, main_menu_view);
  application->registerView(MAP_SELECTION_VIEW, map_selection_view);
  application->registerView(GAMEPLAY_VIEW, gameplay_view);

  /* Activate the Main Menu View */
  application->activateView(MAIN_MENU_VIEW);

  std::exception* caught_exception {nullptr};

  /* Begin the keyboard-event loop. Pressing <q> will terminate the program */
  try {
    application->mainloop('q');
  } catch (std::exception& e) { // Catch and store exception to be propagated after cleaning up.
    caught_exception = new std::exception(e);
  } catch (int code) {
    caught_exception = new std::runtime_error(std::to_string(code));
  } catch (...) {
    caught_exception = new std::runtime_error("unknown error");
  }

  cleanup();

  if (caught_exception != nullptr) {
    std::cerr << caught_exception->what() << std::endl; // Propagate exception
    delete caught_exception;
  }

  return 0;
}
