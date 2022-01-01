
#include "TileWindow.hh"
#include "Components.hh"
#include "Constants.hh"
#include "raygui.h"
#include <algorithm>
#include <cmath>
#include <stdint.h>

namespace {
  int roundDownTo10(int n) {
    // Smaller multiple
    int a = (n / 10) * 10;

    // Larger multiple
    int b = a + 10;

    // Return of closest of two
    return (n - a > b - n) ? b : a;
  }
};// namespace

TileWindow::TileWindow(entt::registry &registry, IFileOpener &fileOpener)
    : registry_(registry), fileOpener_(fileOpener) {
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


void TileWindow::addNewTile(Vector2 const &mousePosition, Rectangle const &dimensions) {
  auto view = registry_.view<Components::TileTexture, Components::Tiles>();

  Components::Tiles::Tile tile;
  tile.dimensions = dimensions;
  tile.position = mousePosition;

  view.each([this, tile](auto &texture, auto &tiles) {
    texture.texture = selectedTileSet_->texture;
    tiles.tiles.emplace_back(tile);
  });
}


bool TileWindow::render() {
  if (GuiWindowBox(windowBoundary_, "Tile debugger")) {
    return false;
  }

  mousepressed_ = IsMouseButtonPressed(0);

  auto gridColorbutton = Rectangle{windowBoundary_.x + 10.f, windowBoundary_.y + 32.f, 100.f, 30.f};

  showGridColor_ = GuiToggle(gridColorbutton, "Grid color", showGridColor_);

  if (showGridColor_) {
    Rectangle colorWindow = {gridColorbutton.x + 16.f, gridColorbutton.y + 32.f, 200.f, 200.f};
    GuiWindowBoxNoClose(colorWindow, "Grid Color");
    gridColor_ = GuiColorPicker({colorWindow.x + 10.f, colorWindow.y + 34.f, 150.f, 150.f}, gridColor_);
  }

  Rectangle windowRect = {0, 0, Constants::screenWidth, Constants::screenHeight};
  int oldStyle = GuiGetStyle(DEFAULT, LINE_COLOR);
  GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(gridColor_));
  auto mouseGridPosition = GuiGrid(windowRect, 10.f, 2);
  GuiSetStyle(DEFAULT, LINE_COLOR, oldStyle);

  float tileBoxHeight = 325.f;
  Rectangle tileBox = {
          windowBoundary_.x + 5.f,
          windowBoundary_.height - tileBoxHeight,
          windowBoundary_.width - 10.f,
          tileBoxHeight - 5.f};

  if (tilesetError_ != TilesetErrors::no_error) {
    GuiDisable();
  }

  drawTileSetSection(tileBox);

  if (tilesetError_ != TilesetErrors::no_error) {
    GuiEnable();
  }

  showTilesetError(tileBox);

  if (selectedFrameIndex_ != -1 && selectedTileSet_) {
    auto mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, windowRect)) {

      // snapping to the grid
      mousePosition.x = (float) roundDownTo10(static_cast<int>(mousePosition.x));
      mousePosition.y = (float) roundDownTo10(static_cast<int>(mousePosition.y));

      auto &tileFrame = selectedTileSet_->frames[selectedFrameIndex_];
      DrawTextureRec(selectedTileSet_->texture, tileFrame.frameDimensions, mousePosition, ColorAlpha(WHITE, 0.6f));

      if (mousepressed_) {
        addNewTile(mousePosition, tileFrame.frameDimensions);
      }
    }
  }

  return true;
}

void TileWindow::drawTileSetSection(Rectangle const &tileBox) {
  GuiGroupBox(tileBox, "Tilesets");

  int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  const char *text = "Browse";
  int size = MeasureText(text, fontSize);

  Rectangle panelRect = {tileBox.x + 10.f, tileBox.y + 50.f, tileBox.width - 20.f, tileBox.height - 95.f};
  Rectangle panelContentRect = {0, 0, tileBox.width - 35.f, 0};

  GuiLabel({(tileBox.x + tileBox.width) - (size + 45.f) - 110.f - 120.f, tileBox.y + 10.f, 120.f, 30.f}, "Select tileset file:");

  if (GuiDropdownBox({(tileBox.x + tileBox.width) - (size + 45.f) - 120.f, tileBox.y + 10.f, 110.f, 30.f}, "aseprite", &parseMethodChosen_, chooseParseMethod_)) {
    chooseParseMethod_ = !chooseParseMethod_;
  }

  tileParser_ = selectParser(parseMethodChosen_);

  if (GuiButton({(tileBox.x + tileBox.width) - (size + 50.f), tileBox.y + 10.f, size + 45.f, 30.f}, text)) {
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

    BeginScissorMode(
      static_cast<int>(view.x),
      static_cast<int>(view.y),
      static_cast<int>(view.width), 
      static_cast<int>(view.height)
    );
    size_t i = 0;
    for (auto &tileFrame : selectedTileSet_->frames) {
      Vector2 position;
      size_t xIndex = tileFrame.index % rowSize;
      size_t yIndex = tileFrame.index / rowSize;

      position.x = panelRect.x + panelScroller_.x + outerXPadding + (xIndex * (tileSpacing + tileFrame.frameDimensions.width));
      position.y = panelRect.y + panelScroller_.y + outerYPadding + (yIndex * (tileSpacing + tileFrame.frameDimensions.height));
      DrawTextureRec(selectedTileSet_->texture, tileFrame.frameDimensions, position, WHITE);

      Rectangle tileRect = {
              position.x,
              position.y,
              tileFrame.frameDimensions.width,
              tileFrame.frameDimensions.height};
      if (mousepressed_ && CheckCollisionPointRec(GetMousePosition(), tileRect)) {
        selectedFrameIndex_ = (i == selectedFrameIndex_) ? -1 : static_cast<int>(i);
        selectedFrameSample_ = tileRect;
      }
      i++;
    }
    if (selectedFrameIndex_ != -1) {
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