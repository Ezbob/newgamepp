
#pragma once

#include <string>
#include <vector>
#include "raylib.h"

#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"

struct TileSet {
  std::string image_name;
  std::string image_path;

  Texture texture;
  size_t width;
  size_t height;

  struct TileFrame {
    size_t index;
    float x;
    float y;
    float width;
    float height;

    template<typename Archive>
    void serialize(Archive &ar) {
      ar(index, x, y, width, height);
    }
  };
  std::vector<TileFrame> frames;

  template<typename Archive>
  void serialize(Archive &ar) {
    ar(image_name, image_path, width, height, frames);
  }
};