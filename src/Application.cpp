#include "Application.h"

View::View() {
}

View::~View() {
}

void View::registerListener(ActionListener* action_listener) {
  _action_listeners.push_back(action_listener);
}

void View::deregisterListener(ActionListener *action_listener) {
  _action_listeners.remove(action_listener);
}

void View::notifyKeyboardEventPerformed(int key) {
  for (auto listener : _action_listeners) {
    if (listener->keyboardEventPerformed(key)) break;
  }
}

void View::notifyActivated() {
  for (auto listener : _action_listeners)
  {
    listener->viewActivated();
  }
}

void View::notifyDeactivated() {
  for (auto listener : _action_listeners)
  {
    listener->viewDeactivated();
  }
}

void View::display() {}

void View::activate() {
  notifyActivated();
}

void View::deactivate() {
  notifyDeactivated();
}

Application::Application() {
}

Application::~Application() {
}

int Application::get_key(bool block) {
  nodelay(stdscr, !block);

  int key = getch();

  nodelay(stdscr, false);

  return key;
}

void Application::mainloop(char esc_key) {
  curs_set(0);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  std::stringstream escape_msg;
  escape_msg << "Press " << esc_key << " to exit";
  printw((escape_msg).str().c_str());
  refresh();

  if (_activeView)
    _activeView->display();

  int ch;
  while ((ch = get_key(false)) != esc_key)
  {
    if (_activeView)
      _activeView->notifyKeyboardEventPerformed(ch);
  }
}

void Application::registerView(int view_id, View* view) {
  _registered_views.insert(std::make_pair(view_id, view));
}

void Application::deregisterView(int view_id){
  _registered_views.erase(view_id);
}

void Application::activateView(int view_id) {
  if (_registered_views.count(view_id)) {
    if (_activeView) {
      _activeView->deactivate();
    }
    _activeView = _registered_views[view_id];
    _activeView->activate();
  }
}

std::shared_ptr<Application> Application::instance()
{
  /*
   * The first time the function is called, _singleton will be
   * instantiated, and then subsequent calls will return that
   * instance.
   */
  static std::shared_ptr<Application>  _singleton(new Application);
  return _singleton;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
  // Implementation based on https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);

  return local_win;
}

WindowView::WindowView() {
  _window = stdscr;
  start_x, start_y = 0;
  getmaxyx(stdscr, height, width);
}

WindowView::WindowView(int w, int h, int x, int y) {
  width = w;
  height = h;
  start_x = x;
  start_y = y;
  _window = create_newwin(h, w, y, x);
}

WindowView::~WindowView()
{
  deactivate();
}

void WindowView::deactivate() {
  if (_window && _window != stdscr)
  {
    wclear(_window);
    wrefresh(_window);
    delwin(_window);
    _window = nullptr;
  }
  View::deactivate();
}

void WindowView::activate() {
  if (_window && _window != stdscr) {
    deactivate();
  }
  View::activate();
  _window = create_newwin(height, width, start_y, start_x);
  display();
}

void WindowView::display() {
  wrefresh(_window);
}

void WindowView::print_centered(int line, std::string msg) {
  int offset = (width - msg.length()) / 2;
  if (offset < 0) offset = 0;
  wmove(_window, line, offset);
  wprintw(_window, msg.c_str());
}

void WindowView::print_centered_at_col(int line, int col, std::string msg) {
  int offset = col - msg.length() / 2;
  if (offset < 0)
    offset = 0;
  wmove(_window, line, offset);
  wprintw(_window, msg.c_str());
}

void WindowView::print_right_aligned_at_col(int line, int col, std::string msg) {
  int offset = col - msg.length();
  if (offset < 0)
    offset = 0;
  wmove(_window, line, offset);
  wprintw(_window, msg.c_str());
}
