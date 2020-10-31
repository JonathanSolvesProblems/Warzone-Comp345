#pragma once
#include <list>
#include <algorithm>
#include "View.h"

class LinearLayout
{
public:
  LinearLayout();
  LinearLayout(int w, int h);
  LinearLayout(std::initializer_list<std::pair<int, View *>>);
  LinearLayout(int w, int h, std::initializer_list<std::pair<int, View *>>);
  ~LinearLayout();

  void addView(View *view);
  void addView(View *view, int weight);

  void removeView(View *view);
  void reweightView(View *view, int new_weight);

  void resize();
  void resize(int w, int h);

protected:
  virtual void recalculateAndResizeView(View *view, float weight, int *offset_out) = 0;

  std::list<std::pair<int, View *>> *_weighted_views;
  int height, width;

private:
  int _getTotalWeight();
  void _recalculateSizes();
  void _deleteAllWindows();
};

class HorizontalLinearLayout : public LinearLayout
{
  protected:
    virtual void recalculateAndResizeView(View* view, float weight, int *offset_out);
};