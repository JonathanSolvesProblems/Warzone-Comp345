#include "Application.h"
#include "Observers.h"
#include <vector>

#define COLOR_GREY 8
#define RED_BLACK 1
#define WHITE_BLACK 2
#define GREY_BLACK 3
#define BLACK_RED 4
#define BLACK_GREEN 5

#define MAIN_MENU_VIEW 0
#define MAP_SELECTION_VIEW 1

class MainMenuView;
class SettingsModel;
class MainMenuController;

class MainMenuController : public ActionListener {
  public:
    MainMenuController(SettingsModel * sm);
    ~MainMenuController();

    virtual bool keyboardEventPerformed(int key);

    virtual void viewActivated();
    virtual void viewDeactivated();

  private:
    SettingsModel *_settings_model;
};

struct SettingsModel {
  ConcreteObservable<bool> phase_headers_enabled;
  ConcreteObservable<bool> stats_headers_enabled;
  ConcreteObservable<int> number_of_players;
};

class MainMenuView : public WindowView, public Observer
{
public:
  MainMenuView(int w, int h, SettingsModel *sm);
  ~MainMenuView();
  virtual void display();
  virtual void update();

private:
  void display_banner(int &offset);
  void display_credits(int &offset);
  void display_menu(int &offset);
  SettingsModel *_settings_model;
};

struct MenuModel {
    ConcreteObservable<std::vector<std::string>> map_file_list;
    ConcreteObservable<int> selected_index;
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
    MapSelectionController(MenuModel *mm);
    ~MapSelectionController();

    virtual bool keyboardEventPerformed(int key);
    virtual void viewActivated();
    virtual void viewDeactivated();

  private:
    MenuModel *_menu_model;
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


class GameplayLayoutView : public View, public Observer
{
  public:
    GameplayLayoutView(View* left_header, View* right_header, View* main_content, SettingsModel* settings_model);

    virtual void display();
    virtual void update();
    virtual void activate();
    virtual void deactivate();
    virtual void notifyKeyboardEventPerformed(int key);

  private:
    const int HEADER_HEIGHT = 6;
    View* left_header;
    View* right_header;
    View* main_content;
    SettingsModel* settings_model;
};