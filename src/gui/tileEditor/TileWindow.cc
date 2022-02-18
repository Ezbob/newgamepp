
#include "TileWindow.hh"
#include "Components.hh"
#include "Constants.hh"

#include "fmt/core.h"
#include "raygui.h"
#include "raymath.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <stdint.h>
#include "TileMap.hh"

#define RAYGUI_CUSTOM_RICONS
#include "ricons.h"


TileWindow::TileWindow(entt::registry &registry, IFileOperations &fileOpener, Camera2D &camera)
    : registry_(registry)
    , tileSetSelector_({windowBoundary_.x + 5.f,
                        windowBoundary_.height - 325.f,
                        windowBoundary_.width - 10.f,
                        325.f - 5.f}, fileOpener)
    , camera_(camera)
    , selected_(registry_)
    , painterTool_(registry_, tileSetSelector_, camera_, selected_, currentLayerId_)
    , removeTool_(registry_, camera_, currentLayerId_, selected_)
    , multiSelectTool_(registry_, currentLayerId_, selected_, camera_)
    , currentTileTool_(&nullTool_)
    , fileOps_(fileOpener) {
  addNewLayer();
  grid_ = registry_.create();
  registry_.emplace<Components::Coloring>(grid_, Fade(GRAY, 0.3f));
  registry_.emplace<Components::Debug>(grid_);
  registry_.emplace<Components::Active>(grid_, false);
  registry_.emplace<Components::Dimensions>(grid_, Constants::screenWidth * 4.f, Constants::screenWidth * 4.f);
}

void TileWindow::addNewLayer() {
  if (selected_.size() > 0) {
    selected_.clear();
  }

  currentLayerId_ = nextLayerId_++;
  layers_.emplace_back(fmt::format("Layer {}", currentLayerId_));
}


void TileWindow::removeLayer() {
  layers_.pop_back();
  nextLayerId_ -= 1;
  if (currentLayerId_ == layers_.size()) {
    currentLayerId_ -= 1;
  }
}


void TileWindow::layerControls() {

  float height = 24.f;

  GuiLabel({windowBoundary_.x + windowBoundary_.width - 160.f - (25.f * 2.f) + 62.f, height + 5.f, 20.f, 20.f}, "Current layer:");

  if (GuiButton({windowBoundary_.x + windowBoundary_.width - 160.f - (25.f * 2.f), height + 27.f, 20.f, 20.f}, "+")) {
    addNewLayer();
  }

  std::vector<const char *> present;
  present.reserve(layers_.size());
  for (auto const &v : layers_) {
    present.emplace_back(v.data());
  }

  int old = currentLayerId_;
  if (GuiDropdownBoxEx({windowBoundary_.x + windowBoundary_.width - 160.f - 25.f, height + 27.f, 150.f, 20.f},
                       present.data(), static_cast<unsigned>(present.size()), &currentLayerId_, layerSelectEditable_)) {
    layerSelectEditable_ = !layerSelectEditable_;
  }

  if (currentLayerId_ == 0) GuiDisable();
  if (GuiButton({windowBoundary_.x + windowBoundary_.width - 30.f, height + 27.f, 20.f, 20.f}, "-")) {
    removeLayer();
  }
  if (currentLayerId_ == 0) GuiEnable();

  if (old != currentLayerId_) {
    selected_.clear();
  }
}


void TileWindow::renderTools(Rectangle const &gridColorbutton) {

  if (!tileSetSelector_.isTileFrameSelected()) GuiDisable();

  Rectangle toolBox = {
          gridColorbutton.x,
          gridColorbutton.y + 60.f,
          (windowBoundary_.width / 2) - 20.f,
          80.f};

  GuiGroupBox(toolBox, "Tile tools");
  Rectangle initialButton = {toolBox.x + 10.f, toolBox.y + 10.f, 30.f, 30.f};

  auto oldSelection = currentTileTool_;

  if (GuiToggle(initialButton,
                GuiIconText(RAYGUI_ICON_BRUSH_PAINTER, nullptr),
                currentTileTool_ == &painterTool_)) {
    currentTileTool_ = &painterTool_;
  }

  if (GuiToggle({initialButton.x + 35.f, initialButton.y, initialButton.width, initialButton.height},
                GuiIconText(RAYGUI_ICON_RUBBER, nullptr),
                currentTileTool_ == &removeTool_)) {
    currentTileTool_ = &removeTool_;
  }

  if (GuiToggle({initialButton.x + (35.f * 2.f), initialButton.y, initialButton.width, initialButton.height},
                GuiIconText(RAYGUI_ICON_CURSOR_SCALE, nullptr),
                currentTileTool_ == &multiSelectTool_)) {
    currentTileTool_ = &multiSelectTool_;
  }

  if (oldSelection != currentTileTool_) {
    currentTileTool_->onSelected();
  }

  if (!tileSetSelector_.isTileFrameSelected()) GuiEnable();
}

void TileWindow::loadFromFile(std::filesystem::path const &path) {
  
}

void TileWindow::saveToFile(std::filesystem::path const &path) {
  TileMap map;

  auto spriteGroup = registry_.group<Components::Renderable>(
    entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>
  );

  for (auto [entity, render, texture, pos, quad, flip] : spriteGroup.each()) {
    TileMap::TilePosition tile;

    tile.alpha = render.alpha;
    tile.layer = render.layer;
    tile.x = pos.x;
    tile.y = pos.y;

    tile.tileSetIndex = texture.loadedIndex;
    tile.tileFrameIndex = quad.index;

    map.tilePositions.push_back(tile);
  }

  auto &tileSets = tileSetSelector_.getSelectedTileSets();
  std::copy(tileSets.begin(), tileSets.end(), std::back_inserter(map.tileSetLocations));

  map.save(path);
}


void TileWindow::renderFileOperations() {
  Rectangle box = {
    windowBoundary_.x + 10.f,
    windowBoundary_.y + 192.f,
    (windowBoundary_.width / 2) - 20.f,
    80.f
  };
  GuiGroupBox(box, "File");

  GuiLabel({box.x + 10.f, box.y + 10.f, 40.f, 25.f}, "Current File:");
  GuiTextBox({box.x + 85.f, box.y + 10.f, 180.f, 25.f}, filePath_.data(), filePath_.size(), false);

  if (path_.empty()) GuiDisable();

  if (GuiButton({box.x + 10.f, box.y + 45.f, 60.f, 25.f}, "Save")) {
    saveToFile(path_);
  }

  if (path_.empty()) GuiEnable();

  if (GuiButton({box.x + 105.f, box.y + 45.f, 70.f, 25.f}, "Save as")) {
    fileOps_.saveFileDialog(path_, "mt");

    if (!path_.has_extension()) {
      path_.replace_extension("mt");
    }
    auto s = path_.filename().string();
    filePath_.assign(s.begin(), s.end());
    filePath_.push_back('\0');
    saveToFile(path_);
  }

  if (GuiButton({box.x + 205.f, box.y + 45.f, 60.f, 25.f}, "Load")) {
    fileOps_.openFileDialog(path_, "mt");
    auto s = path_.filename().string();
    filePath_.assign(s.begin(), s.end());
    filePath_.push_back('\0');
    loadFromFile(path_);
  }
}


bool TileWindow::render() {
  if (GuiWindowBox(windowBoundary_, "Tile debugger")) {
    return false;
  }

  Rectangle gridColorbutton{
          windowBoundary_.x + 10.f,
          windowBoundary_.y + 32.f,
          100.f,
          30.f};

  renderTools(gridColorbutton);

  renderFileOperations();

  Rectangle toolAttribute{
          gridColorbutton.x + (windowBoundary_.width / 2),
          gridColorbutton.y + 60.f,
          (windowBoundary_.width / 2) - 20.f,
          60.f + (40.f * 3.f)};
  GuiGroupBox(toolAttribute, "Tool attributes");

  currentTileTool_->renderToolAttributes(toolAttribute);

  auto &activeGrid = registry_.get<Components::Active>(grid_);
  activeGrid.isActive = GuiCheckBox({gridColorbutton.x + 10.f, 46.f, 15.f, 15.f}, "Toggle grid", activeGrid.isActive);

  tileSetSelector_.render();

  layerControls();

  currentTileTool_->execute();

  if (IsKeyPressed(KEY_DELETE)) {
    selected_.destroy_all();
  }

  return true;
}