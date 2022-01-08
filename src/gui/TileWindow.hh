
#pragma once

#include "AsepriteParser.hh"
#include "Constants.hh"
#include "IRenderable.hh"
#include "ITileParser.hh"
#include "NFDFileOpener.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include <functional>
#include <unordered_map>
#include <vector>

class TileWindow : public IRenderable {
public:
  TileWindow(entt::registry &, IFileOpener &);

  bool render() override;

private:
  Rectangle windowBoundary_{
          Constants::screenWidth,
          0.f,
          600.f,
          Constants::screenHeight + 200.f};

  ITileParser *selectParser(int);

  void openTilesetFile(Rectangle const &);

  void showTilesetError(Rectangle const &);

  void drawTileSetSection(Rectangle const &);

  void addNewTile(Vector2 const &, Rectangle const &);

  void renderTileSet(Rectangle const &);

  inline bool hasLayer() const {
    return registry_.valid(currentLayer_);
  }

  inline bool isTileSelected() const {
    return selectedFrameIndex_ != -1 && selectedTileSet_;
  }

  inline bool hasTileSetError() const {
    return tilesetError_ == TilesetErrors::no_error;
  }

  void addNewLayer();

  void removeLayer();

  void layerControls();

  void removeTile();

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

  enum class TileTool {
    no_tool,
    paint_tool,
    remove_tool
  } tileToolSelected_ = TileTool::paint_tool;

  AsepriteParser aseprite_;

  size_t ids_ = 0;

  bool showGrid_ = true;

  std::unordered_map<std::string, TileSet> tilesets_;

  TileSet *selectedTileSet_ = nullptr;

  int selectedFrameIndex_ = -1;
  Rectangle selectedFrameSample_;

  Vector2 panelScroller_ = {5.f, 5.f};
  IFileOpener &fileOpener_;

  bool layerSelectEditable_ = false;
  int currentLayerIndex_ = -1;
  entt::entity currentLayer_;
  std::vector<const char *> layers_;

  Components::Tiles::Tile surrogateTile_;
};
