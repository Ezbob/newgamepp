
#include "TileWindow.hh"
#include "raygui.h"
#include "Constants.hh"
#include <stdint.h>
#include "nfd.h"


ITileParser *TileWindow::getParser(int index) {
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

void TileWindow::drawTilepanel(Rectangle const& tilebox) {
  const int padding = 10;

  for (auto &tileFrame : selectedTileSet_->frames) {
    Vector2 position;
    position.x = tilebox.x + 10.f + (tileFrame.index * (padding + tileFrame.frameDimensions.width));
    position.y = tilebox.y + 50.f;
    DrawTextureRec(selectedTileSet_->texture, tileFrame.frameDimensions, position, WHITE);
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
  
  int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  mousepressed_ = IsMouseButtonPressed(0);

  auto gridColorbutton = Rectangle{windowBoundary_.x + 10.f, windowBoundary_.y + 32.f, 100.f, 30.f};

  showGridColor_ = GuiToggle(gridColorbutton, "Grid color", showGridColor_);

  if (showGridColor_) {
    Rectangle colorWindow = {gridColorbutton.x + 16.f, gridColorbutton.y + 32.f, 200.f, 200.f};
    GuiWindowBoxNoClose(colorWindow, "Grid Color");
    gridColor_ = GuiColorPicker({colorWindow.x + 10.f, colorWindow.y + 34.f, 150.f, 150.f}, gridColor_);
  }

  int oldStyle = GuiGetStyle(DEFAULT, LINE_COLOR);
  GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(gridColor_));
  auto mouseGridPosition = GuiGrid({0, 0, Constants::screenWidth, Constants::screenHeight}, 10.f, 2);
  GuiSetStyle(DEFAULT, LINE_COLOR, oldStyle);

  if (tilesetError_ != TilesetErrors::no_error) GuiDisable();

  float tileBoxWidth = 325.f;
  auto tileBox = Rectangle{windowBoundary_.x + 5.f, windowBoundary_.height - tileBoxWidth, windowBoundary_.width - 10.f, tileBoxWidth - 5.f};
  GuiGroupBox(tileBox, "Tilesets");

  auto font = GetFontDefault();
  const char *text = "Browse";
  int size = MeasureText(text, fontSize);

  GuiLabel({(tileBox.x + tileBox.width) - (size + 45.f) - 110.f - 120.f, tileBox.y + 10.f, 120.f, 30.f}, "Select tileset file:");

  if (GuiDropdownBox({(tileBox.x + tileBox.width) - (size + 45.f) - 120.f, tileBox.y + 10.f, 110.f, 30.f}, "aseprite", &parseMethodChosen_, chooseParseMethod_)) {
    chooseParseMethod_ = !chooseParseMethod_;
  }

  tileParser_ = getParser(parseMethodChosen_);

  if (!tileParser_) GuiDisable();

  if (GuiButton({(tileBox.x + tileBox.width) - (size + 50.f), tileBox.y + 10.f, size + 45.f, 30.f}, text)) {
    openTilesetFile(tileBox);
  }

  if (tilesets_.size() > 0) {
    drawTilepanel(tileBox);
    {
      std::vector<const char *> labels;
      for (auto &tilesetEntry : tilesets_) {
        labels.push_back(tilesetEntry.first.c_str());
      }

      GuiToggleGroupEx({tileBox.x + 10.f, windowBoundary_.height - 15.f - 30.f, 100.f, 30.f}, labels.size(), labels.data(), 0);
    }
  }

  if (!tileParser_) GuiEnable();

  if (tilesetError_ != TilesetErrors::no_error) GuiEnable();

  showTilesetError(tileBox);

  return true;
}