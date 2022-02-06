
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

#define RAYGUI_CUSTOM_RICONS
#include "ricons.h"


TileWindow::TileWindow(entt::registry &registry, IFileOpener &fileOpener, Camera2D &camera)
    : registry_(registry)
    , tileSelector_({windowBoundary_.x + 5.f,
                    windowBoundary_.height - 325.f,
                    windowBoundary_.width - 10.f,
                    325.f - 5.f}, fileOpener)
    , camera_(camera)
    , painterTool_(registry_, tileSelector_, tileModel_, camera_, selectedTile_)
    , removeTool_(registry_, camera_, currentLayerId_)
    , pickerTool_(registry_, camera_, currentLayerId_, selectedTile_, tileModel_)
    , multiSelectTool_(registry_, currentLayerId_, selectedTiles_, camera_)
    , currentTileTool_(&nullTool_) {
  addNewLayer();
  grid_ = gridModel_.create(registry_);
}



void TileWindow::addNewLayer() {
  if (registry_.valid(selectedTile_)) {
    selectedTile_ = entt::null;
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

  if (GuiDropdownBoxEx({windowBoundary_.x + windowBoundary_.width - 160.f - 25.f, height + 27.f, 150.f, 20.f},
                       present.data(), static_cast<unsigned>(present.size()), &currentLayerId_, layerSelectEditable_)) {
    layerSelectEditable_ = !layerSelectEditable_;
  }

  if (currentLayerId_ == 0) GuiDisable();
  if (GuiButton({windowBoundary_.x + windowBoundary_.width - 30.f, height + 27.f, 20.f, 20.f}, "-")) {
    removeLayer();
  }
  if (currentLayerId_ == 0) GuiEnable();
}


void TileWindow::renderTools(Rectangle const& gridColorbutton) {

  if (!tileSelector_.isTileFrameSelected()) GuiDisable();

  Rectangle toolBox = {
          gridColorbutton.x,
          gridColorbutton.y + 60.f,
          (windowBoundary_.width / 2) - 20.f,
          80.f};

  GuiGroupBox(toolBox, "Tile tools");
  Rectangle initialButton = {toolBox.x + 10.f, toolBox.y + 10.f, 30.f, 30.f};
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
                  GuiIconText(RAYGUI_ICON_COLOR_PICKER, nullptr),
                  currentTileTool_ == &pickerTool_)) {
    currentTileTool_ = &pickerTool_;
  }

  if (GuiToggle({initialButton.x + (35.f * 3.f), initialButton.y, initialButton.width, initialButton.height},
                  GuiIconText(RAYGUI_ICON_CURSOR_SCALE, nullptr),
                  currentTileTool_ == &multiSelectTool_)) {
    currentTileTool_ = &multiSelectTool_;
  }

  if (!tileSelector_.isTileFrameSelected()) GuiEnable();
}


void TileWindow::renderTileAttributes(Rectangle const& tileAttributesBox) {
  if (!registry_.valid(selectedTile_)) GuiDisable();

  GuiGroupBox(tileAttributesBox, "Tile attributes");

  GuiSpinnerEx({tileAttributesBox.x + 55.f, tileAttributesBox.y + 10.f, 125.f, 20.f}, "Alpha:", &tileModel_.alpha, 0.f, 1.f, 0.01f, false);

  auto oldTextAlign = GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT);

  GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

  tileModel_.hFlip = GuiCheckBox({tileAttributesBox.x + 90.f, tileAttributesBox.y + 10.f + (25.f * 1.f), 20.f, 20.f}, "Horizontal Flip:", tileModel_.hFlip);

  tileModel_.vFlip = GuiCheckBox({tileAttributesBox.x + 90.f, tileAttributesBox.y + 10.f + (25.f * 2.f), 20.f, 20.f}, "Vertical Flip:", tileModel_.vFlip);

  GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, oldTextAlign);

  tileModel_.layerIndex = currentLayerId_;

  if (!registry_.valid(selectedTile_)) GuiEnable();
}


bool TileWindow::render() {
  if (GuiWindowBox(windowBoundary_, "Tile debugger")) {
    return false;
  }

  Rectangle gridColorbutton = {windowBoundary_.x + 10.f, windowBoundary_.y + 32.f, 100.f, 30.f};

  renderTools(gridColorbutton);

  renderTileAttributes({
    gridColorbutton.x + (windowBoundary_.width / 2),
    gridColorbutton.y + 60.f,
    (windowBoundary_.width / 2) - 20.f,
    60.f + (40.f * 2.f)
  });

  float height = 46.f;
  gridModel_.show = GuiCheckBox({gridColorbutton.x + 10.f, height, 15.f, 15.f}, "Toggle grid", gridModel_.show);

  /*
  showGridColor_ = GuiToggle(gridColorbutton, "Grid color", showGridColor_);

  if (showGridColor_) {
    Rectangle colorWindow = {gridColorbutton.x + 16.f, gridColorbutton.y + 32.f, 200.f, 200.f};
    GuiWindowBoxNoClose(colorWindow, "Grid Color");
    gridModel_.color = GuiColorPicker({colorWindow.x + 10.f, colorWindow.y + 34.f, 150.f, 150.f}, gridModel_.color);
  }
  */

  tileSelector_.render();

  layerControls();

  currentTileTool_->execute();

  tileModel_.update(registry_, selectedTile_);
  gridModel_.update(registry_, grid_);

  return true;
}