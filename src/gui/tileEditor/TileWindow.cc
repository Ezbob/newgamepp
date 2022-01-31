
#include "TileWindow.hh"
#include "Components.hh"
#include "Constants.hh"

#include "Archetypes.hh"
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

namespace {
  int roundDownTo(int N, int n) {
    assert(N > 0);
    // Smaller multiple
    int a = (n / N) * N;

    // Larger multiple
    int b = a + N;

    // Return of closest of two
    return (n - a > b - n) ? b : a;
  }

  Vector2 midPoint(Rectangle r) {
    return {
            r.x - (r.width / 2),
            r.y - (r.height / 2)};
  }

};// namespace


TileWindow::TileWindow(entt::registry &registry, IFileOpener &fileOpener, Camera2D &camera)
    : registry_(registry)
    , tileSelector_({windowBoundary_.x + 5.f,
                    windowBoundary_.height - 325.f,
                    windowBoundary_.width - 10.f,
                    325.f - 5.f}, fileOpener)
    , camera_(camera)
    , painterTool_(registry_, tileSelector_, tileModel_, camera_, selectedTile_)
    , removeTool_(registry_, camera_, currentLayerId_)
    , pickerTool_(registry_, camera_, currentLayerId_, selectedTile_, tileModel_) {
  addNewLayer();
  grid_ = gridModel_.create(registry_);
}


entt::entity TileWindow::findClickedTile(entt::registry &reg, int layerIndex) {
  auto mouse = GetScreenToWorld2D(GetMousePosition(), camera_);

  auto spriteGroup = reg.group<Components::Renderable>(
          entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>);

  auto it = std::find_if(spriteGroup.rbegin(), spriteGroup.rend(), [this, &mouse, &spriteGroup, layerIndex](entt::entity entity) {
    auto sprite = spriteGroup.get<Components::SpriteTexture>(entity);
    auto render = spriteGroup.get<Components::Renderable>(entity);
    auto position = spriteGroup.get<Components::Position>(entity);
    auto quad = spriteGroup.get<Components::Quad>(entity);

    bool is_colliding = CheckCollisionPointRec(mouse,
                                               {position.x,
                                                position.y,
                                                quad.quad.width,
                                                quad.quad.height});
    return layerIndex == render.layer && is_colliding;
  });

  if (it != spriteGroup.rend()) {
    return *it;
  } else {
    return entt::null;
  }
}


void TileWindow::addNewLayer() {
  if (registry_.valid(selectedTile_)) {
    selectedTile_ = entt::null;
  }

  currentLayerId_ = ids_++;
  layers_.emplace_back(fmt::format("Layer {}", currentLayerId_));
}


void TileWindow::removeLayer() {
  layers_.pop_back();
  ids_ -= 1;
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
  if (
          GuiToggle(
                  initialButton,
                  GuiIconText(RAYGUI_ICON_BRUSH_PAINTER, nullptr),
                  tileToolSelected_ == TileTool::paint_tool)) {
    tileToolSelected_ = TileTool::paint_tool;
  }
  if (
          GuiToggle(
                  {initialButton.x + 35.f, initialButton.y, initialButton.width, initialButton.height},
                  GuiIconText(RAYGUI_ICON_RUBBER, nullptr),
                  tileToolSelected_ == TileTool::remove_tool)) {
    tileToolSelected_ = TileTool::remove_tool;
  }
  if (
          GuiToggle(
                  {initialButton.x + (35.f * 2.f), initialButton.y, initialButton.width, initialButton.height},
                  GuiIconText(RAYGUI_ICON_COLOR_PICKER, nullptr),
                  tileToolSelected_ == TileTool::tile_picker_tool)) {
    tileToolSelected_ = TileTool::tile_picker_tool;
  }

  if (!tileSelector_.isTileFrameSelected()) GuiEnable();
}


void TileWindow::doTools() {
  Rectangle windowRect = {0, 0, Constants::screenWidth, Constants::screenHeight};

  if (tileToolSelected_ == TileTool::paint_tool && tileSelector_.isTileFrameSelected()) {
    painterTool_.execute();
  } else if (tileToolSelected_ == TileTool::remove_tool && tileSelector_.isTileFrameSelected()) {
    removeTool_.execute();
  } else if (tileToolSelected_ == TileTool::tile_picker_tool) {
    pickerTool_.execute();
  }
}

void TileWindow::renderTileAttributes(Rectangle const& tileAttributesBox) {
  if (!registry_.valid(selectedTile_)) GuiDisable();

  GuiGroupBox(tileAttributesBox, "Tile attributes");

  GuiSpinnerEx({tileAttributesBox.x + 55.f, tileAttributesBox.y + 10.f, 125.f, 20.f}, "Alpha:", &tileModel_.alpha, 0.f, 1.f, 0.01f, false);

  GuiSpinner({tileAttributesBox.x + 55.f, tileAttributesBox.y + 10.f + 25.f, 125.f, 20.f}, "Z Index:", &tileModel_.zIndex, -100, 100, false);

  auto oldTextAlign = GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT);

  GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

  tileModel_.hFlip = GuiCheckBox({tileAttributesBox.x + 90.f, tileAttributesBox.y + 10.f + (25.f * 2.f), 20.f, 20.f}, "Horizontal Flip:", tileModel_.hFlip);

  tileModel_.vFlip = GuiCheckBox({tileAttributesBox.x + 90.f, tileAttributesBox.y + 10.f + (25.f * 3.f), 20.f, 20.f}, "Vertical Flip:", tileModel_.vFlip);

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

  gridModel_.show = GuiCheckBox({gridColorbutton.x + 120.f, gridColorbutton.y + 7.5f, 15.f, 15.f}, "Toggle grid", gridModel_.show);

  showGridColor_ = GuiToggle(gridColorbutton, "Grid color", showGridColor_);

  if (showGridColor_) {
    Rectangle colorWindow = {gridColorbutton.x + 16.f, gridColorbutton.y + 32.f, 200.f, 200.f};
    GuiWindowBoxNoClose(colorWindow, "Grid Color");
    gridModel_.color = GuiColorPicker({colorWindow.x + 10.f, colorWindow.y + 34.f, 150.f, 150.f}, gridModel_.color);
  }

  tileSelector_.render();

  layerControls();

  doTools();

  tileModel_.update(registry_, selectedTile_);
  gridModel_.update(registry_, grid_);

  auto currentMouse = GetMousePosition();
  if (IsMouseButtonDown(1)) {
    auto delta = Vector2Subtract(prevMouse_, currentMouse);
    camera_.target = Vector2Add(camera_.target, delta);
  }
  prevMouse_ = currentMouse;

  return true;
}