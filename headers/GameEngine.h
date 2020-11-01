#include "Observers.h"
#include <ncurses.h>
#include <menu.h>
#include <list>
#include <string>
#include <sstream>

#define COLOR_GREY 8
#define RED_BLACK 1
#define WHITE_BLACK 2
#define GREY_BLACK 3
#define BLACK_RED 4
#define BLACK_GREEN 5

class ActionListener;
class View;
class WindowView;
class MainMenuView;
class MainGameModel;
class MainGameController;

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
  virtual void display();

protected:
  virtual void notifyKeyboardEventPerformed(int key);

private:
  std::list<ActionListener *> _action_listeners;
};

WINDOW *create_newwin(int height, int width, int starty, int startx);

class WindowView : public View {
  public:
    WindowView();
    WindowView(int w, int h, int x, int y);
    ~WindowView();
    virtual void display();
    virtual void mainloop(char esc_key);
    void print_centered(int line, std::string msg);
    void deactivate();
    void activate();

  protected:
    int width, height, start_x, start_y;
    WINDOW* _window{nullptr};
};

class MainMenuView : public WindowView, public Observer
{
  public:
    MainMenuView(int w, int h, MainGameModel* mgm);
    ~MainMenuView();
    virtual void display();
    virtual void update();
  private:
    void display_banner(int& offset);
    void display_credits(int& offset);
    void display_menu(int &offset);
    MainGameModel *_main_game_model;
};

class MainGameController : public ActionListener {
  public:
    MainGameController(MainGameModel * mgm);
    ~MainGameController();

    virtual bool keyboardEventPerformed(int key);

  private:
    MainGameModel *_main_game_model;
};

class MainGameModel {
  public:
    ConcreteObservable<int> *getPhase();
    ConcreteObservable<bool> *getPhaseHeadersEnabled();
    ConcreteObservable<bool> *getStatsHeadersEnabled();
    void setPhase(int p);
    void setPhaseHeadersEnabled(bool e);
    void setStatsHeadersEnabled(bool e);

  private:
    ConcreteObservable<int> phase;
    ConcreteObservable<bool> phase_headers_enabled;
    ConcreteObservable<bool> stats_headers_enabled;
};