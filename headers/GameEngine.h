#include "MapLoader.h"
#include "Application.h"
#include "GameObservers.h"
#include "Player.h"
#include <vector>
#include <list>

#define COLOR_GREY 8
#define RED_BLACK 1
#define WHITE_BLACK 2
#define GREY_BLACK 3
#define BLACK_RED 4
#define BLACK_GREEN 5

#define MAIN_MENU_VIEW 0
#define MAP_SELECTION_VIEW 1
#define GAMEPLAY_VIEW 2

class Player;

enum Phase {STARTUP, REINFORCEMENT, ISSUE_ORDERS, ORDERS_EXECUTION};

// forward declarations
class MainMenuView;
class GameModel;
class MainMenuController;

class MainMenuController : public ActionListener {
  public:
    MainMenuController(GameModel * sm);
    ~MainMenuController();

    virtual bool keyboardEventPerformed(int key);

    virtual void viewActivated();
    virtual void viewDeactivated();

  private:
    GameModel *_settings_model;
};

struct GameModel {
  // Basic Settings
  ConcreteObservable<bool> phase_headers_enabled;
  ConcreteObservable<bool> stats_headers_enabled;
  ConcreteObservable<int> number_of_players;

  ConcreteObservable<Phase> current_phase;

  ConcreteObservable<Player*> current_player;
  VectorObservable<Player*> active_players;

  map::Map* map;
};


class MainMenuView : public WindowView, public Observer
{
public:
  MainMenuView(int w, int h, GameModel *sm);
  ~MainMenuView();
  virtual void display();
  virtual void update();

private:
  void display_banner(int &offset);
  void display_credits(int &offset);
  void display_menu(int &offset);
  GameModel *_settings_model;
};

struct MenuModel {
    ConcreteObservable<std::vector<std::string>> map_file_list;
    ConcreteObservable<int> selected_index;
    ConcreteObservable<std::string> error_message;
    void incrementItem(int inc);
    std::string getSelection();
};

class MapMenuView : public WindowView, public Observer {
  public:
    MapMenuView(int w, int h, MenuModel *mm);
    ~MapMenuView();
    virtual void display();
    virtual void update();
  private:
    MenuModel *_menu_model;
};

class MapSelectionView : public WindowView {
  public:
    MapSelectionView(int w, int h, int header_height, MenuModel *mm);
    ~MapSelectionView();

    virtual void display();
    virtual void activate();
    virtual void deactivate();
    virtual void notifyKeyboardEventPerformed(int key);
    void registerMenuListener(ActionListener *listener);
  private:
    MapMenuView* _menu_view{nullptr};
};

class MapSelectionController : public ActionListener {
  public:
    MapSelectionController(MenuModel *mm, GameModel *gm);
    ~MapSelectionController();

    virtual bool keyboardEventPerformed(int key);
    virtual void viewActivated();
    virtual void viewDeactivated();

  private:
    MenuModel *_menu_model;
    GameModel *_game_model;
};

class MapMenuController : public ActionListener
{
  public:
    MapMenuController(MenuModel *mm);
    ~MapMenuController();

    virtual bool keyboardEventPerformed(int key);
    virtual void viewActivated();
    virtual void viewDeactivated();

  private:
    MenuModel *_menu_model;
};

/*
observes the phase that the player is on and which player it is.
*/
class PhaseObserverView : public WindowView, public Observer
{
  public:
    PhaseObserverView(int w, int h, int x, int y, GameModel *gm);
    ~PhaseObserverView();

    virtual void update();

    virtual void display();

private:
    GameModel * _game_model;
};

/*
Statistics observer for keeping track of the percentage of the territories that each player owns.
The players that are still in game. If they lose, they will be removed from the statisticsobserver.
*/
class StatisticsObserverView : public WindowView, public Observer 
{
public:
  StatisticsObserverView(int w, int h, int x, int y, GameModel *gm);
  ~StatisticsObserverView();

  virtual void update();

  virtual void display();

private:
    GameModel * _game_model;
};

class GameplayView : public WindowView {

  public:
    GameplayView(int w, int h, GameModel *sm);
    ~GameplayView();
    virtual void display();
    virtual void activate();
    virtual void deactivate();

  private:
    void create_phase_observer_view();
    void create_stats_observer_view();
    PhaseObserverView* _phase_view{nullptr};
    StatisticsObserverView* _stats_view{nullptr};
    GameModel* settings_model{nullptr};
};

class GameplayController : public ActionListener {
  public:
    GameplayController(GameModel *gm);
    ~GameplayController();

    virtual bool keyboardEventPerformed(int key);

    virtual void viewActivated();
    virtual void viewDeactivated();

  private:
    void startupPhase();
    void mainGameLoop();

    GameModel *_game_model;
};