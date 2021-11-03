
#pragma once

#include <vector>
#include "raylib.h"
#include <string>

class TextField {

public:
  TextField(Rectangle &parent, Rectangle bounds);

  void render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex = 1);
  void render(bool mousepressed, Vector2 &mousepos, std::string &data, int stackIndex = 1);

private:
  bool editable_ = false;
  Rectangle &parent_;
  Rectangle bounds_;
  std::vector<char> buffer_;
};