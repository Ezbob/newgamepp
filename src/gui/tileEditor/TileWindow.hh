
#pragma once

#include <functional>
#include <vector>

#include "Constants.hh"
#include "IRenderable.hh"
#include "ITileParser.hh"
#include "MultiSelectTool.hh"
#include "NFDFileOpener.hh"
#include "NullTileTool.hh"
#include "PainterTool.hh"
#include "RemoveTool.hh"
#include "SelectedCollection.hh"
#include "TileSetSelector.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"

class TileWindow : public IRenderable {
public:
  TileWindow(entt::registry &, IFileOperations &, Camera2D &);

  bool render() override;

private:
  Rectangle windowBoundary_{
          Constants::screenWidth,
          0.f,
          600.f,
          Constants::screenHeight + 180.f};

  void addNewLayer();

  void removeLayer();

  void layerControls();

  void renderTools(Rectangle const &);

  void renderFileOperations();

  void saveToFile(std::filesystem::path const &);
  void loadFromFile(std::filesystem::path const &);

  std::vector<char> filePath_ = {'\0'};
  std::filesystem::path path_;

  entt::registry &registry_;

  TileSetSelector tileSetSelector_;

  bool showGridColor_ = false;

  int nextLayerId_ = 0;
  int currentLayerId_ = 0;
  std::vector<std::string> layers_;
  bool layerSelectEditable_ = false;

  SelectedCollection selected_;

  entt::entity grid_;
  Camera2D &camera_;

  NullTileTool nullTool_;
  PainterTool painterTool_;
  RemoveTool removeTool_;
  MultiSelectTool multiSelectTool_;

  ITileTool *currentTileTool_;
  IFileOperations &fileOps_;
};
