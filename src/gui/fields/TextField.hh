
#pragma once

#include <array>
#include "raylib.h"

class TextField {

public:
  TextField(Rectangle &parent, Rectangle bounds);

  void render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex = 1);
  void render(bool mousepressed, Vector2 &mousepos, int &data, int stackIndex = 1);

private:
  bool editable_ = false;
  Rectangle &parent_;
  Rectangle bounds_;
  std::array<char, 16> buffer_;
};