
#pragma once

#include "TileSetSelector.hh"
#include "raygui.h"
#include "raylib.h"
#include <algorithm>

TileSetSelector::TileSetSelector(Rectangle r, IFileOperations &fo) : boundary_(r), fileOpener_(fo) {}

void TileSetSelector::nextTileSet() {
  selectedTileSet_ = &tilesets_.back();
  selectedTileSetIndex_ = (tilesets_.size() - 1);
}

TileSetSelector::TilesetErrors TileSetSelector::loadTileSet(TileSet &tileset) {
  std::string id = tileset.image_name;

  auto it = std::find_if(tilesets_.begin(), tilesets_.end(), [&id](TileSet const& t) {
    return id == t.image_name;
  });

  if (it != tilesets_.end()) {
    return TilesetErrors::tileset_already_loaded;
  }

  if (!std::filesystem::exists(tileset.image_path)) {
    return TilesetErrors::file_not_found;
  }

  tileset.texture = LoadTexture(tileset.image_path.c_str());
  if (!(tileset.texture.height == static_cast<int>(tileset.height) ||
      tileset.texture.width == static_cast<int>(tileset.width))) {
    return TilesetErrors::tileset_load_error;
  }

  tilesets_.push_back(tileset);
  nextTileSet();

  return TilesetErrors::no_error;
}


void TileSetSelector::openTilesetFile(Rectangle const &tileBox) {
  const char *extensions = tileParser_->getFileExtensions();// a semi-colon seperated list of extensions
  std::filesystem::path path;

  if (extensions && fileOpener_.openFileDialog(path, extensions)) {
    auto parsedResults = tileParser_->parse(path);

    if (std::holds_alternative<TileSet>(parsedResults)) {
      auto tilesetDefinition = std::get<TileSet>(parsedResults);
      auto fault = loadTileSet(tilesetDefinition);

      if (fault != TilesetErrors::no_error) {
        tilesetError_ = fault;
      }
    }
  }
}


ITileParser *TileSetSelector::selectParser(int index) {
  switch (index) {
    case 0:
      return static_cast<ITileParser *>(&aseprite_);
  }
  return nullptr;
}


void TileSetSelector::drawTileSetSection(Rectangle const &tileBox) {
  GuiGroupBox(tileBox, "Tilesets");

  int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);

  Rectangle panelRect = {tileBox.x + 10.f, tileBox.y + 50.f, tileBox.width - 20.f, tileBox.height - 95.f};
  Rectangle panelContentRect = {0, 0, tileBox.width - 35.f, 0};

  GuiLabel({(tileBox.x + tileBox.width) - 65.f - 110.f - 120.f, tileBox.y + 10.f, 120.f, 30.f}, "Select tileset file:");

  if (GuiDropdownBox({(tileBox.x + tileBox.width) - 70.f - 120.f, tileBox.y + 10.f, 110.f, 30.f}, "aseprite", &parseMethodChosen_, chooseParseMethod_)) {
    chooseParseMethod_ = !chooseParseMethod_;
  }

  tileParser_ = selectParser(parseMethodChosen_);

  bool mousePressed = IsMouseButtonPressed(0);

  if (GuiButton({(tileBox.x + tileBox.width) - (25.f + 50.f), tileBox.y + 10.f, 20.f + 45.f, 30.f}, "Browse")) {
    openTilesetFile(tileBox);
  }

  if (tilesets_.size() <= 0 && !selectedTileSet_) {
    return;
  }

  const size_t tileSpacing = 10;
  const size_t outerYPadding = 5;
  const size_t outerXPadding = 5;

  size_t tileWidth = 0, tileHeight = 0;
  if (selectedTileSet_->frames.size() > 0) {
    auto &frames = selectedTileSet_->frames;

    for (auto const &frame : frames) {
      if (frame.width > tileWidth) {
        tileWidth = (size_t) frame.width;
      }
      if (frame.height > tileHeight) {
        tileHeight = (size_t) frame.height;
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
  for (auto const &tileFrame : selectedTileSet_->frames) {
    Vector2 position;
    size_t xIndex = tileFrame.index % rowSize;
    size_t yIndex = tileFrame.index / rowSize;

    position.x = panelRect.x + panelScroller_.x + outerXPadding + (xIndex * (tileSpacing + tileFrame.width));
    position.y = panelRect.y + panelScroller_.y + outerYPadding + (yIndex * (tileSpacing + tileFrame.height));

    DrawTextureRec(selectedTileSet_->texture, {tileFrame.x, tileFrame.y, tileFrame.width, tileFrame.height}, position, tileColor);

    Rectangle tileRect = {
            position.x,
            position.y,
            tileFrame.width,
            tileFrame.height};
    if (isGuiNormal && mousePressed && CheckCollisionPointRec(GetMousePosition(), tileRect)) {
      SelectedTileFrame selectedFrame = {
        *selectedTileSet_,
        static_cast<int>(i),
        selectedTileSetIndex_
      };

      selectedFrame_.emplace(selectedFrame);
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
      labels.push_back(tilesetEntry.image_name.c_str());
    }

    selectedTileSetIndex_ = GuiToggleGroupEx({
      tileBox.x + 10.f,
      (boundary_.y + boundary_.height) - 10.f - 30.f,
      maxWidth, 30.f
    }, static_cast<unsigned>(labels.size()), labels.data(), selectedTileSetIndex_);
  }
}


void TileSetSelector::render() {
  if (!hasTileSetError()) {
    GuiDisable();
  }

  drawTileSetSection(boundary_);

  if (!hasTileSetError()) {
    GuiEnable();
  }

  showTilesetError(boundary_);
}


void TileSetSelector::showTilesetError(Rectangle const &tileBox) {
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