#include <curses.h>
#include <list>
#include <string>
#include <sstream>
#include <memory>
#include <unordered_map>

class ActionListener;
class Application;
class View;
class WindowView;

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

  virtual void viewActivated() = 0;
  virtual void viewDeactivated() = 0;
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
  virtual void notifyActivated();
  virtual void notifyDeactivated();

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
    void registerView(int view_id, View* view);
    void deregisterView(int view_id);
    void activateView(int view_id);

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
