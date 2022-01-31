
#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

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

  struct TileTextures {
    std::unordered_map<std::string, Texture> textures;
  };

  struct Flipable {
    bool hFlipped = false;
    bool vFlipped = false;
  };

  struct Active {
    bool isActive;
  };

  struct Renderable {
    float alpha = 1.0;
    int layer = 1;
  };

  struct SpriteTexture {
    Texture texture;
  };

  struct Quad {
    Rectangle quad;
  };

  struct Debug {};

  struct Coloring {
    Color color;
  };

};// namespace Components