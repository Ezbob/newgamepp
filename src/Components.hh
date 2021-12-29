
#pragma once

#include <string>
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

  struct TileTexture {
    Texture texture;
  };

  struct Tiles {
    struct Tile {
      int zIndex = 0;
      float alpha = 1.0;
      Vector2 position;
      Rectangle dimensions;
    };
    std::vector<Tile> tiles;
  };

};// namespace Components