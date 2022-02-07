
#pragma once

#include "AsepriteParser.hh"
#include "IFileOpener.hh"
#include "ITileParser.hh"
#include "TileSet.hh"
#include "raylib.h"
#include <optional>
#include <unordered_map>

class TileSetSelector {
public:
  TileSetSelector(Rectangle, IFileOpener &);

  void render();

  inline bool hasTileSetError() const {
    return tilesetError_ == TilesetErrors::no_error;
  }

  inline bool isTileFrameSelected() const {
    return selectedFrame_.has_value();
  }


private:
  Rectangle boundary_;

  void showTilesetError(Rectangle const &);

  void drawTileSetSection(Rectangle const &);

  std::optional<TileSet> openTilesetFile(Rectangle const &);

  void renderTileSet(Rectangle const &);

  ITileParser *selectParser(int);

  IFileOpener &fileOpener_;

  enum class TilesetErrors {
    no_error,
    file_not_found,
    tileset_already_loaded,
    tileset_parse_error
  } tilesetError_ = TilesetErrors::no_error;


  bool chooseParseMethod_ = false;

  int parseMethodChosen_ = 0;

  ITileParser *tileParser_;

  Vector2 panelScroller_ = {5.f, 5.f};

  std::unordered_map<std::string, TileSet> tilesets_;

  AsepriteParser aseprite_;

  Rectangle selectedFrameSample_;

  struct SelectedTileFrame {
    TileSet &set;
    int frameIndex;
  };

  std::optional<TileSet> selectedTileSet_;
  std::optional<SelectedTileFrame> selectedFrame_;

public:
  inline std::optional<SelectedTileFrame> getSelectedFrame() const {
    return selectedFrame_;
  }
};