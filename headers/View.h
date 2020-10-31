#pragma once
#include <ncurses.h>

WINDOW *create_new_window(int height, int width, int starty, int startx);

class View
{
public:
  View();
  View(int width, int height, int startx, int starty);
  ~View();
  void update();
  virtual void draw() = 0;
  void refresh();
  void clear();
  void destroyWindow();
  void createWindow(int width, int height, int startx, int starty);

private:
  WINDOW *window{nullptr};
};
