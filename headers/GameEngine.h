#include "MapLoader.h"
#include "Application.h"
#include "GameObservers.h"
#include "Player.h"
#include <vector>
#include <list>
#include <math.h>

#ifdef __linux__
#include <unistd.h>
#else
#include <Windows.h>
#endif

// Define constants to represent color pairs to be used with NCurses
#define COLOR_GREY 8
#define RED_BLACK 1
#define WHITE_BLACK 2
#define GREY_BLACK 3
#define BLACK_RED 4
#define BLACK_GREEN 5


// Define constants to represent View ids
#define MAIN_MENU_VIEW 0
#define MAP_SELECTION_VIEW 1
#define GAMEPLAY_VIEW 2

class Player;

// All possible game phases
enum Phase {STARTUP, REINFORCEMENT, ISSUE_ORDERS, ORDERS_EXECUTION};

// Converts Phase enum to a std::string for display
string convertEnum(Phase current_phase);

// forward declarations
class MainMenuView;
class GameModel;
class MainMenuController;

/* Handles keyboard events on the main menu, updates game settings */
class MainMenuController : public ActionListener {
  public:
    /* Accepts a pointer to the GameModel used to store the settings */
    MainMenuController(GameModel * sm);
    ~MainMenuController();

    /* Toggles the game observer headers, and navigates to the next view */
    virtual bool keyboardEventPerformed(int key);
    
    /* Not used */
    virtual void viewActivated();

    /* Not used */
    virtual void viewDeactivated();
    

  private:
    /* Pointer to settings */
    GameModel *_settings_model;
};

/* A POD to store Observables used by the game */
struct GameModel {
  GameModel();
  ~GameModel();

  // Basic Settings
  ConcreteObservable<bool>* phase_headers_enabled{nullptr};
  ConcreteObservable<bool> *stats_headers_enabled{nullptr};
  ConcreteObservable<int> *number_of_players{nullptr};

  // Tracks the current game phase
  ConcreteObservable<Phase> *current_phase{nullptr};

  /*
   * Tracks the current player.
   * Note: It only updates when the pointer changes, not when
   * the contents of the Player object change
   */
  ConcreteObservable<Player *> *current_player{nullptr};

  /*
   * Tracks the currently active players.
   * Updates when the underlying vector changes, i.e adding/removing
   * AND when the Players themselves change.
   */
  VectorObservable<Player *> *active_players{nullptr};

  /*
   * Not Observable, but necessary for gameplay none the less
   */
  map::Map* map {nullptr};


  /*
   * An observable message log
   */
  StringLog* log {nullptr};
};


/*
 * Draws the Main Menu: logo, instructions, settings, and credits.
 */
class MainMenuView : public WindowView, public Observer
{
public:
  /* Create a window with the specified width and height, and obtain a reference to the GameModel */
  MainMenuView(int w, int h, GameModel *sm);
  ~MainMenuView();

  /* Draw the menu */
  virtual void display();

  /* Redraw when one of the observables is updated */
  virtual void update();

private:
  /* Draws the logo */
  void display_banner(int &offset);
  /* Draws the credits */
  void display_credits(int &offset);
  /* Draws the game settings */
  void display_menu(int &offset);

  /* Reference to the GameModel used throughout the game */
  GameModel *_settings_model;
};

/* Structure to hold data necessary for rendering the map selection */
struct MenuModel {
    MenuModel();
    ~MenuModel();
    /* Stores the list of files in the directory */
    ConcreteObservable<std::vector<std::string>>* map_file_list;

    /* Stores the index of the currently selected file */
    ConcreteObservable<int>* selected_index;

    /* Stores an error message to be shown, when empty, no message is shown */
    ConcreteObservable<std::string>* error_message;

    /* Increments the currently selected index, and wraps around */
    void incrementItem(int inc);

    /* Get the name of the currently selected file */
    std::string getSelection();
};

/* Draws the list of files and error message if necessary */
class MapMenuView : public WindowView, public Observer {
  public:
    /* Creates the menu with the specified height and width */
    MapMenuView(int w, int h, MenuModel *mm);
    ~MapMenuView();

    /* Draws the menu */
    virtual void display();

    /* Redraw when any of the observables change */
    virtual void update();
  private:
    MenuModel *_menu_model;
};


/* Draws the map menu instructions, and manages the MapMenuView */
class MapSelectionView : public WindowView {
  public:
    /* Creates two windows: a header for instructions, with specified height,
    and a MapMenuView with height h - header_height */
    MapSelectionView(int w, int h, int header_height, MenuModel *mm);

    /* Destroys the MapMenuView */
    ~MapSelectionView();

    /* Draws the header, and the menu */
    virtual void display();

    /* Notifies the MapMenuView that it has been activated */
    virtual void activate();

    /* Notifies the MapMenuView that it has been deactivated */
    virtual void deactivate();

    /* Passes up/down arrow key events to the MapMenuView */
    virtual void notifyKeyboardEventPerformed(int key);

    /* Registers an ActionListener on the MapMenuView */
    void registerMenuListener(ActionListener *listener);
  private:
    MapMenuView* _menu_view{nullptr};
};


/* Handles loading and validating the selected map */
class MapSelectionController : public ActionListener {
  public:
    /* Obtain references to the necessary models */
    MapSelectionController(MenuModel *mm, GameModel *gm);
    ~MapSelectionController();

    /* Attempts to load the currently selected map when SPACE is pressed
    Sets an error message in model if map load is not successful.
    */
    virtual bool keyboardEventPerformed(int key);

    /* Loads the list of views when the view is first activated */
    virtual void viewActivated();

    /* does nothing */
    virtual void viewDeactivated();

  private:
    MenuModel *_menu_model;
    GameModel *_game_model;
};


/* Selects an element of the map_file_list using arrow keys */
class MapMenuController : public ActionListener
{
  public:
    /* Obtain reference to menu model */
    MapMenuController(MenuModel *mm);
    ~MapMenuController();

    /* Increment / decrement selection index when key is pressed */
    virtual bool keyboardEventPerformed(int key);

    /* does nothing */
    virtual void viewActivated();
    /* does nothing */
    virtual void viewDeactivated();

  private:
    MenuModel *_menu_model;
};

/*
Displays the current game phase and the name of the current player
*/
class PhaseObserverView : public WindowView, public Observer
{
  public:
    /* Creates a window with the specified dimensions, and attaches
    to the phase observable and the current player observable */
    PhaseObserverView(int w, int h, int x, int y, GameModel *gm);
    ~PhaseObserverView();

    /* Redraws when any of the observables update */
    virtual void update();

    /* Draws the current phase and player name */
    virtual void display();

private:
    GameModel * _game_model;
};

/*
Statistics observer for keeping track of the percentage of the territories that each player owns.
The players that are still in game. If they lose, they will be removed from the statistics observer.
Displays a congratulatory message when a player wins.
*/
class StatisticsObserverView : public WindowView, public Observer 
{
public:
  /* Creates a window with the specified dimensions and attaches to the active players observable */
  StatisticsObserverView(int w, int h, int x, int y, GameModel *gm);
  ~StatisticsObserverView();

  /* Redraws when the observable updates */
  virtual void update();

  /* Displays a graph showing each players control of the world, and a message when a player wins */
  virtual void display();

private:
    GameModel * _game_model;
};


/* Manages the game observers and displays a log of recent game events */
class GameplayView : public WindowView, public Observer {

  public:
    /* Create the game play view with the specified width and height, attach to
    the log observable */
    GameplayView(int w, int h, GameModel *sm);
    ~GameplayView();

    /* Draw as many recent events as will fit in the window */
    virtual void display();

    /* Redraw when the log is appended to or clear */
    virtual void update();

    /* Notify sub views of activation, and notify controller */
    virtual void activate();
    /* Notify sub views of deactivation, and notify controller */
    virtual void deactivate();

  private:
    /* Create the PhaseObserverView with the correct dimensions */
    void create_phase_observer_view(int header_height);
    /* Create the stats observer view with the correct dimensions */
    void create_stats_observer_view(int header_height);
    PhaseObserverView* _phase_view{nullptr};
    StatisticsObserverView* _stats_view{nullptr};
    GameModel* settings_model{nullptr};
};

/* Manages all the core game logic: start up phase, main game loop, etc... */
class GameplayController : public ActionListener {
  public:
    /* Obtain reference to game model */
    GameplayController(GameModel *gm);
    
    /* Destroy everything that was created by the controller (Players, etc...) */
    ~GameplayController();

    /* Currently no keyboard input is used */
    virtual bool keyboardEventPerformed(int key);

    /* Begins the game by calling startupPhase, then calls mainGameLoop */
    virtual void viewActivated();

    /* Handles clean up */
    virtual void viewDeactivated();
    

  private:
    /* Creates the required number of players, determines a random order of play,
    and assigns territories */
    void startupPhase();

    /* Executes each phase in the following sequence until the game is complete:
    REINFORCEMENT, ISSUE_ORDERS, EXECUTE_ORDERS
    */
    void mainGameLoop();

    bool somePlayerHasArmies();

    /* Randomly assigns territories to players in round-robin order */
    void assign_territories();

    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();

    bool reinforcementsAvailable();
    int getPlayersBonus(Player* p);

    void deploySubPhase();
    void airliftSubPhase();
    void blockadeSubPhase();

    void advanceSubPhase();
     void bombSubPhase();
    // void negotiateSubPhase();

    GameModel *_game_model;
};