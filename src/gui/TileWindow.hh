
#pragma once

#include "Constants.hh"
#include "IRenderable.hh"
#include "raylib.h"

class TileWindow : public IRenderable {
public:
  TileWindow() = default;

  bool render() override;

private:
  Rectangle windowBoundary_{
          Constants::screenWidth,
          0.f,
          600.f,
          Constants::screenHeight + 200.f};

  Color gridColor_ = GRAY;
  bool mousepressed_ = false;
  bool showGridColor_ = false;
};
