#include "View.h"

WINDOW *create_new_window(int height, int width, int starty, int startx)
{
  // Implementation based on https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
  WINDOW *new_window;
  new_window = newwin(height, width, starty, startx);
  box(new_window, 0, 0);
  wrefresh(new_window);

  return new_window;
}

View::View()
{
  this->window = stdscr;
}

View::View(int width, int height, int startx, int starty)
{
  createWindow(width, height, startx, starty);
}

View::~View()
{
  if (window && window != stdscr)
  {
    wclear(window);
    wrefresh(window);
    delwin(window);
    window = nullptr;
  }
}

void View::update()
{
  clear();
  draw();
  refresh();
}

void View::clear()
{
  wclear(window);
}

void View::refresh()
{
  wrefresh(window);
}

void View::destroyWindow() {
  // Implementation based on https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
  wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(window);
  delwin(window);
  window = nullptr;
}

void View::createWindow(int width, int height, int startx, int starty)
{
  if (window) {
    destroyWindow();
  }
  this->window = create_new_window(height, width, starty, startx);
}