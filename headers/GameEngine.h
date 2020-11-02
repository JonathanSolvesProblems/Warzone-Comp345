#include "Observers.h"
#include <ncurses.h>
#include <menu.h>
#include <list>
#include <string>
#include <sstream>
#include <memory>
#include <unordered_map>

#define COLOR_GREY 8
#define RED_BLACK 1
#define WHITE_BLACK 2
#define GREY_BLACK 3
#define BLACK_RED 4
#define BLACK_GREEN 5

#define MAIN_MENU_VIEW 0

class ActionListener;
class Application;
class View;
class WindowView;
class MainMenuView;
class GameModel;
class GameController;

class ActionListener
{
public:
  /*
    * Called whenever a keyboard event is detected.
    * 
    * Returns true if the event has been processed, and false
    * if the event should be propagated to the next listener
    * in the chain.
  */
  virtual bool keyboardEventPerformed(int key) = 0;
};

class View
{
public:
  View();
  ~View();
  void registerListener(ActionListener *action_listener);
  void deregisterListener(ActionListener *action_listener);
  virtual void activate();
  virtual void deactivate();
  virtual void display();
  virtual void notifyKeyboardEventPerformed(int key);

private:
  std::list<ActionListener *> _action_listeners;
};

class Application
{
  public:
    Application(const Application&) = delete;
    ~Application();
    static std::shared_ptr<Application> instance();
    void mainloop(char esc_key);
    void activateView(int view_id);
    void registerView(int view_id, View* view);
    void deregisterView(int view_id);

  private:
    Application();
    View* _activeView{nullptr};
    std::unordered_map<int, View*> _registered_views;
};

WINDOW *create_newwin(int height, int width, int starty, int startx);

class WindowView : public View {
  public:
    WindowView();
    WindowView(int w, int h, int x, int y);
    ~WindowView();
    virtual void display();
    virtual void activate();
    virtual void deactivate();
    void print_centered(int line, std::string msg);

  protected:
    int width, height, start_x, start_y;
    WINDOW* _window{nullptr};
};

class MainMenuView : public WindowView, public Observer
{
  public:
    MainMenuView(int w, int h, GameModel* mgm);
    ~MainMenuView();
    virtual void display();
    virtual void update();
  private:
    void display_banner(int& offset);
    void display_credits(int& offset);
    void display_menu(int &offset);
    GameModel *_game_model;
};

class GameController : public ActionListener {
  public:
    GameController(GameModel * mgm);
    ~GameController();

    virtual bool keyboardEventPerformed(int key);

  private:
    GameModel *_game_model;
};

class GameModel {
  public:
    ConcreteObservable<bool> *getPhaseHeadersEnabled();
    ConcreteObservable<bool> *getStatsHeadersEnabled();
    void setPhaseHeadersEnabled(bool e);
    void setStatsHeadersEnabled(bool e);

  private:
    ConcreteObservable<bool> phase_headers_enabled;
    ConcreteObservable<bool> stats_headers_enabled;
};

class SelectMapView : public View, public Observer {
  private:

};

class SelectMapController : public ActionListener {
  SelectMapController();
  ~SelectMapController();
};