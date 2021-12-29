
#pragma once

#include "Constants.hh"
#include "IRenderable.hh"
#include "ITileParser.hh"
#include "raylib.h"
#include "AsepriteParser.hh"
#include <vector>
#include <unordered_map>
#include "entt/entity/registry.hpp"

class TileWindow : public IRenderable {
public:
  TileWindow(entt::registry &);

  bool render() override;

private:
  Rectangle windowBoundary_{
          Constants::screenWidth,
          0.f,
          600.f,
          Constants::screenHeight + 200.f};

  ITileParser *selectParser(int);

  void openTilesetFile(Rectangle const&);

  void showTilesetError(Rectangle const&);

  void drawTileSetSection(Rectangle const&);

  entt::registry &registry_;

  bool chooseParseMethod_ = false;

  int parseMethodChosen_ = 0;
  ITileParser *tileParser_;

  Color gridColor_ = GRAY;
  bool mousepressed_ = false;
  bool showGridColor_ = false;

  enum class TilesetErrors {
    no_error,
    file_not_found,
    tileset_already_loaded,
    tileset_parse_error
  } tilesetError_ = TilesetErrors::no_error;

  AsepriteParser aseprite_;

  size_t ids_ = 0;

  std::unordered_map<std::string, TileSet> tilesets_;

  TileSet *selectedTileSet_ = nullptr;

  int selectedFrameIndex_ = -1;
  Rectangle selectedFrameSample_;

  Vector2 panelScroller_ = {5.f, 5.f};
};
