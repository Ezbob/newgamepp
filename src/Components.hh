
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "raylib.h"

namespace Components {

  struct Position {
    float x;
    float y;
  };

  struct Dimensions {
    float w;
    float h;
  };

  struct Velocity {
    float dx;
    float dy;
  };

  struct Name {
    std::string name;
  };

  struct ScreenClamp {
    float left = 0.2f;
    float right = 0.2f;
    float bottom = 0.2f;
    float top = 0.2f;
  };

  struct ZIndex {
    int index;
  };

  struct TileTextures {
    std::unordered_map<std::string, Texture> textures;
  };

  struct Tiles {
    struct Tile {
      std::string const *textureName;
      float alpha = 1.0;
      Vector2 position;
      Rectangle dimensions;
    };
    std::vector<Tile> tiles;
  };

  struct Active {
    bool isActive;
  };

};// namespace Components