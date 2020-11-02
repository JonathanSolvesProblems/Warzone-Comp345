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

void View::display() {}

void View::activate() {}

void View::deactivate() {}

Application::Application() {
}

Application::~Application() {
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
  while ((ch = getch()) != esc_key)
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
}

void WindowView::activate() {
  if (_window && _window != stdscr) {
    deactivate();
  }
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
