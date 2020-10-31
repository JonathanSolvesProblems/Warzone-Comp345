#include "Layout.h"

LinearLayout::LinearLayout()
{
  _weighted_views = new std::list<std::pair<int, View *>>();
  getmaxyx(stdscr, height, width);
}

LinearLayout::LinearLayout(int w, int h)
{
  _weighted_views = new std::list<std::pair<int, View *>>();
  width = w;
  height = h;
}

LinearLayout::LinearLayout(std::initializer_list<std::pair<int, View *>> views_initializer)
{
  _weighted_views = new std::list<std::pair<int, View *>>(views_initializer);
  getmaxyx(stdscr, height, width);
}

LinearLayout::LinearLayout(int w, int h, std::initializer_list<std::pair<int, View *>> views_initializer)
{
  _weighted_views = new std::list<std::pair<int, View *>>(views_initializer);
  width = w;
  height = h;
}

LinearLayout::~LinearLayout()
{
  delete _weighted_views;
}

void LinearLayout::addView(View *view)
{
  addView(view, 1);
}

void LinearLayout::addView(View *view, int weigth)
{
  _weighted_views->push_back(std::make_pair(weigth, view));
  resize();
}

void LinearLayout::removeView(View *view)
{
  _weighted_views->remove_if([view](std::pair<int, View *> pair) {
    return pair.second == view;
  });
  resize();
}

void LinearLayout::reweightView(View *view, int new_weight)
{
  auto pair = std::find_if(_weighted_views->begin(), _weighted_views->end(), [view](std::pair<int, View *> pair) {
    return pair.second == view;
  });

  if (pair != _weighted_views->end())
  {
    pair->first = new_weight;
  }
}

int LinearLayout::_getTotalWeight() {
  int total = 0;
  for (auto pair : *_weighted_views) {
    total += pair.first;
  }
  if (total == 0) {
    total = 1;
  }
  return total;
}

void LinearLayout::resize() {
  int w, h;
  getmaxyx(stdscr, h, w);
  resize(w, h);
}

void LinearLayout::resize(int w, int h) {
  _deleteAllWindows();
  _recalculateSizes();
}

void LinearLayout::_deleteAllWindows() {
  for (auto pair : *_weighted_views) {
    if (pair.second) {
      pair.second->destroyWindow();
    }
  }
}

void LinearLayout::_recalculateSizes() {
  int total_weight = _getTotalWeight();
  
  int* offset = new int(0);
  for (auto pair : *_weighted_views) {
    float weight = pair.first / (float) total_weight;
    recalculateAndResizeView(pair.second, weight, offset);
  }
  delete offset;
}

void HorizontalLinearLayout::recalculateAndResizeView(View* view, float weight, int *offset_out) {
  if (view) {
    int view_width = this->width * weight;
    int view_height = this->height;
    view->createWindow(view_width, view_height, *offset_out, 0);
    *offset_out += view_width;
  }
}
