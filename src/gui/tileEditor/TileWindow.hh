
#pragma once


#include "Constants.hh"
#include "GridModel.hh"
#include "IRenderable.hh"
#include "ITileParser.hh"
#include "NFDFileOpener.hh"
#include "TileModel.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "TileSelector.hh"
#include <functional>
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

  void addNewLayer();

  void removeLayer();

  void layerControls();

  void removeTile();

  void renderTools(Rectangle &);

  void doTools();

  entt::registry &registry_;

  TileSelector tileSelector_;

  bool mousepressed_ = false;
  bool showGridColor_ = false;

  enum class TileTool {
    no_tool,
    paint_tool,
    remove_tool,
    tile_picker_tool
  } tileToolSelected_ = TileTool::paint_tool;

  int ids_ = 0;
  int currentLayerId_ = 0;
  std::vector<std::string> layers_;
  bool layerSelectEditable_ = false;

  entt::entity selectedTile_;

  TileModel tileModel_;
  GridModel gridModel_;

  entt::entity grid_;
};
