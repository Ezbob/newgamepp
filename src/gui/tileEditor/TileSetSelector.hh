
#pragma once

#include "AsepriteParser.hh"
#include "IFileOperations.hh"
#include "ITileParser.hh"
#include "TileSet.hh"
#include "raylib.h"
#include <optional>
#include <unordered_map>

class TileSetSelector {
public:
  enum class TilesetErrors {
    no_error,
    file_not_found,
    tileset_already_loaded,
    tileset_load_error,
    tileset_parse_error
  } tilesetError_ = TilesetErrors::no_error;


  TileSetSelector(Rectangle, IFileOperations &);

  void render();

  inline bool hasTileSetError() const {
    return tilesetError_ == TilesetErrors::no_error;
  }

  inline bool isTileFrameSelected() const {
    return selectedFrame_.has_value();
  }

  TilesetErrors loadTileSet(TileSet &tileset);

private:
  Rectangle boundary_;

  void nextTileSet();

  void showTilesetError(Rectangle const &);

  void drawTileSetSection(Rectangle const &);

  void openTilesetFile(Rectangle const &);

  void renderTileSet(Rectangle const &);

  ITileParser *selectParser(int);

  IFileOperations &fileOpener_;

  bool chooseParseMethod_ = false;

  int parseMethodChosen_ = 0;

  ITileParser *tileParser_ = nullptr;

  Vector2 panelScroller_ = {5.f, 5.f};

  std::vector<TileSet> tilesets_;

  AsepriteParser aseprite_;

  Rectangle selectedFrameSample_;

  struct SelectedTileFrame {
    TileSet &set;
    int frameIndex;
    int tileSetIndex;
  };

  int selectedTileSetIndex_ = 0;
  TileSet *selectedTileSet_ = nullptr;
  std::optional<SelectedTileFrame> selectedFrame_;

public:
  inline std::vector<TileSet> &getLoadedTileSets() {
    return tilesets_;
  }

  inline std::optional<SelectedTileFrame> &getSelectedFrame() {
    return selectedFrame_;
  }
};