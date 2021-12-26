
#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "raylib.h"

struct TileSet {
  std::string image_name;
  std::filesystem::path image_path;

  Texture texture;
  size_t width;
  size_t height;

  struct TileFrame {
    size_t index;
    Rectangle frameDimensions;
    bool rotated;
    bool trimmed;
  };
  std::vector<TileFrame> frames;
};