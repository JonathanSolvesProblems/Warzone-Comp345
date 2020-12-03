#pragma once

/*
 * Application.h
 * 
 * Contains the basis for an Model-View-Controller based UI
 * implemented with the NCurses (PDCurses on Windows) library.
 */

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

/*
 * The ActionListener interface represents the Controller in MVC.
 * Derived classes may register as listeners to Views in order
 * to be notified of keyboard events, or when the view is activated
 * or deactivated. This interface was initially inspired by the Java
 * ActionListener interface.
 */
class ActionListener
{
public:
  /*
    * Called whenever a keyboard event is detected.
    * 
    * Arguments:
    *   int key: The NCurses key code for the key that was pressed.
    * 
    * Returns true if the event has been processed, and false
    * if the event should be propagated to the next listener
    * in the chain.
  */
  virtual bool keyboardEventPerformed(int key) = 0;

  // Called when the view is activated by the Application
  virtual void viewActivated() = 0;

  // Called when the view is deactivated.
  virtual void viewDeactivated() = 0;
};

/*
 * The View class encapsulates general View behaviours such as
 * registering and notifying ActionListeners of view events, separately
 * from any actual GUI framework.
 */
class View
{
public:
  View();
  ~View();

  /* Register an ActionListener to receive view events */
  void registerListener(ActionListener *action_listener);

  /* Deregister an ActionListener to stop receiving view events */
  void deregisterListener(ActionListener *action_listener);

  /* Called by the Application when this view should become active */
  virtual void activate();

  /* Called by the Application before activating the next view */
  virtual void deactivate();

  /*
   * This method is called when a representation of the view needs
   * to be output.
   */
  virtual void display();

  /*
   * When a key is pressed, the Application::mainloop calls this method
   * on the currently active view to notify it of a keyboard event.
   * The view is then responsible to notify it's listeners and/or propagate
   * the event to any sub-views.
   * 
   * If an ActionListener::keyboardEventPerformed method returns `true` then
   * the next ActionListener in the list WILL NOT be notified. This was intended
   * to allow certain events to be intercepted by a general game-wide controller (i.e
   * keyboard shortcuts that should work on all Views) however we did not take
   * advantage of this functionality in this part of the assignment.
   * 
   * Arguments:
   *    int key: The NCurses keycode for the key that was pressed.
   */
  virtual void notifyKeyboardEventPerformed(int key);

  /* Notifies _all_ registered ActionListeners that the view has been activated. */
  virtual void notifyActivated();

  /* Notifies _all_ registered ActionListeners that the view has been activated. */
  virtual void notifyDeactivated();

private:
  /* Maintains an ordered list of registered ActionListeners */
  std::list<ActionListener *> _action_listeners;
};

/*
 * The Application class uses the Singleton design pattern to provide
 * a single source for keyboard inputs and activating and deactivating
 * Views. In this way it can be thought of a sort of "page router." 
 */
class Application
{
  public:
    /*
     * Because there should only ever be ONE instance of Application,
     * the copy constructor has been disabled
     */
    Application(const Application&) = delete;
    ~Application();

    /*
     * Returns a managed pointer to the Application instance.
     * 
     * Because destructing a singleton is non-trivial, we decided to
     * implement it using a shared pointer so that the instance will
     * automatically be deleted once there are no more references to
     * it.
     */
    static std::shared_ptr<Application> instance();

    /*
     * This is the entry point to any application built using this framework.
     * It initiates NCurses and begins the main keyboard event loop.
     * 
     * Arguments:
     *   char esc_key: When this key is pressed the application will terminate.
     */
    void mainloop(char esc_key);

    int get_key(bool block);

    /* Register a View to an id so that it can be activated from any controller */
    void registerView(int view_id, View* view);

    /* Deregister a View by id */
    void deregisterView(int view_id);

    /*
     * Activate the View corresponding to the specified ID.
     * 
     * Conceptually, it can be thought of as "navigating" the Application
     * to the page represented by the View.
     */
    void activateView(int view_id);

  private:
    // Because we are using the singleton pattern, the constructor is private.
    Application();

    // Maintain a refrence to the currently active View.
    View* _activeView{nullptr};

    // Maintain a mapping from view ID to View object.
    std::unordered_map<int, View*> _registered_views;
};

/*
 * Creates an NCurses window with the specified height, width, and start position.
 * 
 * The start position corresponds to the top-left corner.
 */
WINDOW *create_newwin(int height, int width, int starty, int startx);


/*
 * An view implementation which uses the NCurses terminal GUI library
 * to display.
 */
class WindowView : public View {
  public:
    WindowView();
    /*
     * Creates an NCurses window with the specified dimensions
     * 
     * Arguments:
     *  int w: The width of the window in columns
     *  int h: The height of the window in rows
     *  int x: The column in the terminal of the top-left corner of the window
     *  int y: The row in the terminal of the top-left corner of the window
     */
    WindowView(int w, int h, int x, int y);

    // Handles deleting the NCurses window and clearing the terminal space it used.
    ~WindowView();

    // Redraws the NCurses window
    virtual void display();

    /*
     * Creates the NCurses window on activation according to the dimensions
     * specified in the constructor.
     */
    virtual void activate();

    // Destroys the NCurses window
    virtual void deactivate();

    /*
     * A helper method to print a single-line string in the center of the
     * specified row.
     * 
     * Arguments:
     *  int line: The row of the window to draw to
     *  std::string msg: A single-line of text to be displayed.
     */
    void print_centered(int line, std::string msg);
    void print_centered_at_col(int line, int col, std::string msg);
    void print_right_aligned_at_col(int line, int col, std::string msg);

  protected:
    int width, height, start_x, start_y;

    // A reference to the NCurses window which will be drawn to.
    WINDOW* _window{nullptr};
};
