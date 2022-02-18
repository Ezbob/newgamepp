
#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "raylib.h"

#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"

struct TileSet {
  std::string image_name;
  std::filesystem::path image_path;

  Texture texture;
  size_t width;
  size_t height;

  struct TileFrame {
    size_t index;
    Rectangle frameDimensions;

    template<typename Archive>
    void serialize(Archive &ar) {
      ar(index, frameDimensions.x, frameDimensions.y, frameDimensions.width, frameDimensions.height);
    }
  };
  std::vector<TileFrame> frames;

  template<typename Archive>
  void serialize(Archive &ar) {
    ar(image_path.string(), width, height, frames);
  }
};