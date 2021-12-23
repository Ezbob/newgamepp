
#pragma once

#include "Constants.hh"
#include "IRenderable.hh"
#include "ITileParser.hh"
#include "raylib.h"
#include "AsepriteParser.hh"
#include <vector>
#include <unordered_map>

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

  void drawTilepanel(Rectangle const&);

  void openTilesetFile(Rectangle const&);

  bool chooseParseMethod_ = false;
  
  int parseMethodChosen_ = 0;
  ITileParser *tileParser_;

  Color gridColor_ = GRAY;
  bool mousepressed_ = false;
  bool showGridColor_ = false;

  bool showErrorNotFound_ = false;

  AsepriteParser aseprite_;

  size_t ids_ = 0;

  std::unordered_map<std::string, TileSet> tilesets_;

  TileSet *selectedTileSet_ = nullptr;
};
