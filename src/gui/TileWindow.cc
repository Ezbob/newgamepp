
#include "TileWindow.hh"
#include "raygui.h"
#include "Constants.hh"
#include <stdint.h>
#include "nfd.h"
#include <algorithm>
#include <cmath>

namespace {
  int roundDownTo10(int n) {
      // Smaller multiple
      int a = (n / 10) * 10;
      
      // Larger multiple
      int b = a + 10;
  
      // Return of closest of two
      return (n - a > b - n)? b : a;
  }
};


ITileParser *TileWindow::selectParser(int index) {
  switch (index)
  {
    case 0:
      return static_cast<ITileParser *>(&aseprite_);
  }
  return nullptr;
}

void TileWindow::openTilesetFile(Rectangle const& tileBox) {
  nfdchar_t *path = NULL;

  const nfdchar_t *current_directory = GetWorkingDirectory();  
  const char *extensions = tileParser_->getFileExtensions(); // a semi-colon seperated list of extensions

  nfdresult_t result = NFD_OpenDialog(extensions, current_directory, &path);
  if (result == NFD_OKAY) {
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

      tilesetDefinition.texture = LoadTexture(tilesetDefinition.image_path.c_str());

      tilesets_[id] = tilesetDefinition;

      if (!selectedTileSet_) {
        selectedTileSet_ = &tilesets_[id];
      }
    }
    NFD_Path_Free(path);
  } else if (result == NFD_CANCEL) {
    NFD_Path_Free(path);
  }
}


void TileWindow::showTilesetError(Rectangle const& tileBox) {
  static const char *ok_button = "OK";
  Rectangle window_bounds = {tileBox.x + 10 + 160.f, tileBox.y + 10 + 50.f, 250.f, 100.f};

  switch(tilesetError_) {
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
      mousePosition.x = roundDownTo10( mousePosition.x );
      mousePosition.y = roundDownTo10( mousePosition.y );

      auto &tileFrame = selectedTileSet_->frames[selectedFrameIndex_];
      DrawTextureRec(selectedTileSet_->texture, tileFrame.frameDimensions, mousePosition, ColorAlpha(WHITE, 0.6));
    
      if (mousepressed_) {
        // TODO: add tile to some data structure, and add it to the background
      }
    }
  }

  return true;
}

void TileWindow::drawTileSetSection(Rectangle const& tileBox) {
  GuiGroupBox(tileBox, "Tilesets");

  size_t fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
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
        if (frame.frameDimensions.width > tileWidth){
          tileWidth = frame.frameDimensions.width;
        }
        if (frame.frameDimensions.height > tileHeight){
          tileHeight = frame.frameDimensions.height;
        }
      }
    }

    size_t rowSize = tileBox.width / (tileSpacing + tileWidth);
    size_t numberOfRows = selectedTileSet_->frames.size() / rowSize;

    panelContentRect.height += numberOfRows * (tileHeight + tileSpacing); 

    Rectangle view = GuiScrollPanel(panelRect, panelContentRect, &panelScroller_);

    BeginScissorMode(view.x, view.y, view.width, view.height);
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
        tileFrame.frameDimensions.height
      };
      if (mousepressed_ && CheckCollisionPointRec(GetMousePosition(), tileRect)) {
        selectedFrameIndex_ = (i == selectedFrameIndex_) ? -1 : i;
        selectedFrameSample_ = tileRect;
      }
      i++;
    }
    if (selectedFrameIndex_ != -1) {
      DrawRectangleLinesEx({
        selectedFrameSample_.x - 5.f, 
        selectedFrameSample_.y - 5.f, 
        selectedFrameSample_.width + 10.f, 
        selectedFrameSample_.height + 10.f
      }, 1, GREEN);
    }
    EndScissorMode();

    {
      size_t maxWidth = 100.f;
      std::vector<const char *> labels;
      for (auto &tilesetEntry : tilesets_) {
        labels.push_back(tilesetEntry.first.c_str());
      }

      GuiToggleGroupEx({tileBox.x + 10.f, windowBoundary_.height - 15.f - 30.f, maxWidth, 30.f}, labels.size(), labels.data(), 0);
    }

  }

}