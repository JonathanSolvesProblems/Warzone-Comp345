#include "Application.h"
#include "Observers.h"

#define COLOR_GREY 8
#define RED_BLACK 1
#define WHITE_BLACK 2
#define GREY_BLACK 3
#define BLACK_RED 4
#define BLACK_GREEN 5

#define MAIN_MENU_VIEW 0

class MainMenuView;
class GameModel;
class GameController;

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