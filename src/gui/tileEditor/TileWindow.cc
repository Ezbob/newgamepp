
#include "TileWindow.hh"
#include "Components.hh"
#include "Constants.hh"

#include "Archetypes.hh"
#include "fmt/core.h"
#include "raygui.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <stdint.h>

#define RAYGUI_CUSTOM_RICONS
#include "ricons.h"

namespace {
  int roundDownTo10(int n) {
    // Smaller multiple
    int a = (n / 10) * 10;

    // Larger multiple
    int b = a + 10;

    // Return of closest of two
    return (n - a > b - n) ? b : a;
  }

  void drawCornerBox(Rectangle bounds, float thick = 1.f, float lineLength = 10.f, Color color = GREEN) {
    auto endX = bounds.x + bounds.width;
    auto endY = bounds.y + bounds.height;

    // upper left corner
    DrawLineEx({bounds.x, bounds.y}, {bounds.x + lineLength, bounds.y}, thick, color);// horizontal
    DrawLineEx({bounds.x, bounds.y}, {bounds.x, bounds.y + lineLength}, thick, color);// vertical

    // upper right corner
    DrawLineEx({endX, bounds.y}, {endX - lineLength, bounds.y}, thick, color);
    DrawLineEx({endX, bounds.y}, {endX, bounds.y + lineLength}, thick, color);

    // lower left corner
    DrawLineEx({bounds.x, endY}, {bounds.x + lineLength, endY}, thick, color);
    DrawLineEx({bounds.x, endY}, {bounds.x, endY - lineLength}, thick, color);

    // lower right corner
    DrawLineEx({endX, endY}, {endX - lineLength, endY}, thick, color);
    DrawLineEx({endX, endY}, {endX, endY - lineLength}, thick, color);
  }

  Vector2 midPoint(Rectangle r) {
    return {
            r.x - (r.width / 2),
            r.y - (r.height / 2)};
  }

  entt::entity findClickedTile(entt::registry &reg, int layerIndex) {
    auto mouse = GetMousePosition();

    auto spriteGroup = reg.group<Components::Renderable>(
            entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>);

    auto it = std::find_if(spriteGroup.rbegin(), spriteGroup.rend(), [&mouse, &spriteGroup, layerIndex](entt::entity entity) {
      auto sprite = spriteGroup.get<Components::SpriteTexture>(entity);
      auto render = spriteGroup.get<Components::Renderable>(entity);
      auto position = spriteGroup.get<Components::Position>(entity);
      auto quad = spriteGroup.get<Components::Quad>(entity);
      return layerIndex == render.layer &&
             CheckCollisionPointRec(mouse, {
                                                   position.x,
                                                   position.y,
                                                   quad.quad.width,
                                                   quad.quad.height,
                                           });
    });

    return *it;
  }

};// namespace


TileWindow::TileWindow(entt::registry &registry, IFileOpener &fileOpener)
    : registry_(registry), fileOpener_(fileOpener) {
  addNewLayer();
  grid_ = gridModel_.create(registry_);
}


ITileParser *TileWindow::selectParser(int index) {
  switch (index) {
    case 0:
      return static_cast<ITileParser *>(&aseprite_);
  }
  return nullptr;
}


void TileWindow::openTilesetFile(Rectangle const &tileBox) {
  const char *extensions = tileParser_->getFileExtensions();// a semi-colon seperated list of extensions
  std::filesystem::path path;

  if (fileOpener_.openFile(extensions, path)) {
    auto parsedResults = tileParser_->parse(path);

    if (std::holds_alternative<TileSet>(parsedResults)) {
      auto tilesetDefinition = std::get<TileSet>(parsedResults);
      std::string id = tilesetDefinition.image_name;

      if (tilesets_.find(id) != tilesets_.end()) {
        tilesetError_ = TilesetErrors::tileset_already_loaded;
        return;
      }

      if (!std::filesystem::exists(tilesetDefinition.image_path)) {
        tilesetError_ = TilesetErrors::file_not_found;
        return;
      }

      std::string path = tilesetDefinition.image_path.string();
      tilesetDefinition.texture = LoadTexture(path.c_str());

      tilesets_[id] = tilesetDefinition;

      if (!selectedTileSet_) {
        selectedTileSet_ = &tilesets_[id];
      }
    }
  }
}


void TileWindow::showTilesetError(Rectangle const &tileBox) {
  static const char *ok_button = "OK";
  Rectangle window_bounds = {tileBox.x + 10 + 160.f, tileBox.y + 10 + 50.f, 250.f, 100.f};

  switch (tilesetError_) {
    case TilesetErrors::file_not_found: {
      if (GuiMessageBox(window_bounds, "Could not find image", "The associated image could not be found", ok_button) != -1) {
        tilesetError_ = TilesetErrors::no_error;
      }
      break;
    }
    case TilesetErrors::tileset_already_loaded: {
      if (GuiMessageBox(window_bounds, "Tileset already loaded", "The selected tileset is already loaded", ok_button) != -1) {
        tilesetError_ = TilesetErrors::no_error;
      }
      break;
    }
    case TilesetErrors::tileset_parse_error: {
      if (GuiMessageBox(window_bounds, "Tileset parse error", "The selected tileset file could not be parsed", ok_button) != -1) {
        tilesetError_ = TilesetErrors::no_error;
      }
      break;
    }
    default:
      break;
  };
}


void TileWindow::drawTileSetSection(Rectangle const &tileBox) {
  GuiGroupBox(tileBox, "Tilesets");

  int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);

  Rectangle panelRect = {tileBox.x + 10.f, tileBox.y + 50.f, tileBox.width - 20.f, tileBox.height - 95.f};
  Rectangle panelContentRect = {0, 0, tileBox.width - 35.f, 0};

  GuiLabel({(tileBox.x + tileBox.width) - 65.f - 110.f - 120.f, tileBox.y + 10.f, 120.f, 30.f}, "Select tileset file:");

  if (GuiDropdownBox({(tileBox.x + tileBox.width) - 70.f - 120.f, tileBox.y + 10.f, 110.f, 30.f}, "aseprite", &parseMethodChosen_, chooseParseMethod_)) {
    chooseParseMethod_ = !chooseParseMethod_;
  }

  tileParser_ = selectParser(parseMethodChosen_);

  if (GuiButton({(tileBox.x + tileBox.width) - (25.f + 50.f), tileBox.y + 10.f, 20.f + 45.f, 30.f}, "Browse")) {
    openTilesetFile(tileBox);
  }

  if (tilesets_.size() > 0) {
    const size_t tileSpacing = 10;
    const size_t outerYPadding = 5;
    const size_t outerXPadding = 5;

    size_t tileWidth = 0, tileHeight = 0;
    if (selectedTileSet_->frames.size() > 0) {
      auto &frames = selectedTileSet_->frames;

      for (auto const &frame : frames) {
        if (frame.frameDimensions.width > tileWidth) {
          tileWidth = (size_t) frame.frameDimensions.width;
        }
        if (frame.frameDimensions.height > tileHeight) {
          tileHeight = (size_t) frame.frameDimensions.height;
        }
      }
    }

    size_t rowSize = static_cast<size_t>(tileBox.width / (tileSpacing + tileWidth));
    size_t numberOfRows = selectedTileSet_->frames.size() / rowSize;

    panelContentRect.height += numberOfRows * (tileHeight + tileSpacing);

    Rectangle view = GuiScrollPanel(panelRect, panelContentRect, &panelScroller_);
    int guiState = GuiGetState();
    bool isGuiNormal = guiState == GuiControlState::GUI_STATE_NORMAL;
    Color tileColor = (!isGuiNormal) ? ColorAlpha(WHITE, 0.6f) : WHITE;

    BeginScissorMode(
            static_cast<int>(view.x),
            static_cast<int>(view.y),
            static_cast<int>(view.width),
            static_cast<int>(view.height));
    size_t i = 0;
    for (auto &tileFrame : selectedTileSet_->frames) {
      Vector2 position;
      size_t xIndex = tileFrame.index % rowSize;
      size_t yIndex = tileFrame.index / rowSize;

      position.x = panelRect.x + panelScroller_.x + outerXPadding + (xIndex * (tileSpacing + tileFrame.frameDimensions.width));
      position.y = panelRect.y + panelScroller_.y + outerYPadding + (yIndex * (tileSpacing + tileFrame.frameDimensions.height));

      DrawTextureRec(selectedTileSet_->texture, tileFrame.frameDimensions, position, tileColor);

      Rectangle tileRect = {
              position.x,
              position.y,
              tileFrame.frameDimensions.width,
              tileFrame.frameDimensions.height};
      if (isGuiNormal && mousepressed_ && CheckCollisionPointRec(GetMousePosition(), tileRect)) {
        selectedFrameIndex_ = (i == selectedFrameIndex_) ? -1 : static_cast<int>(i);
        selectedFrameSample_ = tileRect;
      }
      i++;
    }
    if (isGuiNormal && isTileFrameSelected()) {
      DrawRectangleLinesEx({selectedFrameSample_.x - 5.f,
                            selectedFrameSample_.y - 5.f,
                            selectedFrameSample_.width + 10.f,
                            selectedFrameSample_.height + 10.f},
                           1, GREEN);
    }
    EndScissorMode();

    {
      float maxWidth = 100.f;
      std::vector<const char *> labels;
      for (auto &tilesetEntry : tilesets_) {
        labels.push_back(tilesetEntry.first.c_str());
      }

      GuiToggleGroupEx({tileBox.x + 10.f, windowBoundary_.height - 15.f - 30.f, maxWidth, 30.f}, static_cast<unsigned>(labels.size()), labels.data(), 0);
    }
  }
}

void TileWindow::addNewLayer() {
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


void TileWindow::removeTile() {
  auto entity = findClickedTile(registry_, currentLayerId_);
  if (registry_.valid(entity)) {
    registry_.destroy(entity);
  }
}

void TileWindow::renderTools(Rectangle &gridColorbutton) {

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
}

void TileWindow::doTools() {
  Rectangle windowRect = {0, 0, Constants::screenWidth, Constants::screenHeight};

  if (isTileFrameSelected() && tileToolSelected_ == TileTool::paint_tool) {
    auto mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, windowRect)) {
      auto &tileFrame = selectedTileSet_->frames[selectedFrameIndex_];

      mousePosition = midPoint({mousePosition.x,
                                mousePosition.y,
                                tileFrame.frameDimensions.width,
                                tileFrame.frameDimensions.height});

      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        // snapping to the grid
        mousePosition.x = (float) roundDownTo10(static_cast<int>(mousePosition.x));
        mousePosition.y = (float) roundDownTo10(static_cast<int>(mousePosition.y));
      }

      DrawTextureRec(
              selectedTileSet_->texture,
              {tileFrame.frameDimensions.x,
               tileFrame.frameDimensions.y,
               tileFrame.frameDimensions.width,
               tileFrame.frameDimensions.height},
              mousePosition,
              ColorAlpha(WHITE, 0.6f));

      if (IsMouseButtonPressed(0)) {

        if (!IsKeyDown(KEY_LEFT_CONTROL)) {
          tileModel_.reset();
        }
        selectedTile_ = tileModel_.create(registry_);

        tileModel_.texture = selectedTileSet_->texture;
        tileModel_.position = mousePosition;
        tileModel_.quad = tileFrame.frameDimensions;
      }
    }
  }

  if (isTileFrameSelected() && tileToolSelected_ == TileTool::remove_tool) {
    if (IsMouseButtonPressed(0)) {
      removeTile();
    }
  }

  if (tileToolSelected_ == TileTool::tile_picker_tool) {
    if (IsMouseButtonPressed(0)) {
      auto mousePosition = GetMousePosition();
      if (CheckCollisionPointRec(mousePosition, windowRect)) {
        if (auto found = findClickedTile(registry_, currentLayerId_); registry_.valid(found)) {
          selectedTile_ = tileModel_.read(registry_, found);
        }
      }
    }
  }
}


bool TileWindow::render() {
  if (GuiWindowBox(windowBoundary_, "Tile debugger")) {
    return false;
  }

  mousepressed_ = IsMouseButtonPressed(0);

  auto gridColorbutton = Rectangle{windowBoundary_.x + 10.f, windowBoundary_.y + 32.f, 100.f, 30.f};

  if (!isTileFrameSelected()) GuiDisable();

  renderTools(gridColorbutton);

  if (!isTileFrameSelected()) GuiEnable();

  if (!registry_.valid(selectedTile_)) GuiDisable();

  Rectangle tileAttributesBox = {
          gridColorbutton.x + (windowBoundary_.width / 2),
          gridColorbutton.y + 60.f,
          (windowBoundary_.width / 2) - 20.f,
          80.f + (40.f * 2.f)};
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


  gridModel_.show = GuiCheckBox({gridColorbutton.x + 120.f, gridColorbutton.y + 7.5f, 15.f, 15.f}, "Toggle grid", gridModel_.show);

  showGridColor_ = GuiToggle(gridColorbutton, "Grid color", showGridColor_);

  if (showGridColor_) {
    Rectangle colorWindow = {gridColorbutton.x + 16.f, gridColorbutton.y + 32.f, 200.f, 200.f};
    GuiWindowBoxNoClose(colorWindow, "Grid Color");
    gridModel_.color = GuiColorPicker({colorWindow.x + 10.f, colorWindow.y + 34.f, 150.f, 150.f}, gridModel_.color);
  }

  if (!hasTileSetError()) {
    GuiDisable();
  }

  Rectangle tileBox = {
          windowBoundary_.x + 5.f,
          windowBoundary_.height - 325.f,
          windowBoundary_.width - 10.f,
          325.f - 5.f};

  drawTileSetSection(tileBox);

  if (!hasTileSetError()) {
    GuiEnable();
  }

  showTilesetError(tileBox);

  layerControls();

  doTools();

  if (registry_.valid(selectedTile_)) {
    tileModel_.update(registry_, selectedTile_);

    auto [pos, dim] = registry_.get<Components::Position, Components::Quad>(selectedTile_);
    drawCornerBox({pos.x, pos.y, dim.quad.width, dim.quad.height});
  }

  if (registry_.valid(grid_)) {
    gridModel_.update(registry_, grid_);
  }

  return true;
}