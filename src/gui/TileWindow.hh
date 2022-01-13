
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

  entt::entity createTile();

  void renderTileSet(Rectangle const &);

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

  void renderTools(Rectangle &);

  void doTools();

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
    remove_tool,
    tile_picker_tool
  } tileToolSelected_ = TileTool::paint_tool;

  AsepriteParser aseprite_;

  int ids_ = 0;
  int currentLayerId_ = 0;
  std::vector<std::string> layers_;
  bool layerSelectEditable_ = false;


  bool showGrid_ = true;

  std::unordered_map<std::string, TileSet> tilesets_;

  TileSet *selectedTileSet_ = nullptr;

  int selectedFrameIndex_ = -1;
  Rectangle selectedFrameSample_;

  Vector2 panelScroller_ = {5.f, 5.f};
  IFileOpener &fileOpener_;

  entt::entity selectedTile_;
  struct TileModel {
    Texture texture;
    int zIndex = 1;
    int layerIndex = 1;
    float alpha = 1.0f;
    Vector2 position = {0, 0};
    Rectangle quad = {0,0,0,0};
    bool vFlip = false;
    bool hFlip = false;

    inline void write_to(entt::registry &reg, entt::entity e) const {
      reg.get<Components::SpriteTexture>(e).texture = texture;

      auto &render = reg.get<Components::Renderable>(e);
      render.alpha = alpha;
      render.layer = layerIndex;
      render.zIndex = zIndex;

      auto &flip = reg.get<Components::Flipable>(e);
      flip.hFlipped = hFlip;
      flip.vFlipped = vFlip;

      auto &pos = reg.get<Components::Position>(e);
      pos.x = position.x;
      pos.y = position.y;

      reg.get<Components::Quad>(e).quad = quad;
    }

    inline entt::entity read_from(entt::registry &reg, entt::entity e) {
      texture = reg.get<Components::SpriteTexture>(e).texture;

      auto &render = reg.get<Components::Renderable>(e);
      alpha = render.alpha;
      layerIndex = render.layer;
      zIndex = render.zIndex;

      auto &flip = reg.get<Components::Flipable>(e);
      flip.hFlipped = hFlip;
      flip.vFlipped = vFlip;

      auto &pos = reg.get<Components::Position>(e);
      position.x = pos.x;
      position.y = pos.y;

      quad = reg.get<Components::Quad>(e).quad;
      return e;
    }

  } tileModel_;

};
