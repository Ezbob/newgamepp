

#pragma once

#include <string>
#include <vector>
#include "raylib.h"

struct TileSet {
  std::string image_name;
  size_t width;
  size_t height;

  struct TileFrame {
    Rectangle frameDimensions;
    bool rotated;
    bool trimmed;
  };
  std::vector<TileFrame> frames;
};