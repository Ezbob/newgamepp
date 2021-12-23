
#pragma once

#include "Constants.hh"
#include "IRenderable.hh"
#include "ITileParser.hh"
#include "raylib.h"
#include "AsepriteParser.hh"

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

  ITileParser *getParser(int);

  void openTilesetFile(void);

  bool chooseParseMethod_ = false;
  
  int parseMethodChosen_ = 0;
  ITileParser *tileParser_;

  Color gridColor_ = GRAY;
  bool mousepressed_ = false;
  bool showGridColor_ = false;

  AsepriteParser aseprite_;
};
