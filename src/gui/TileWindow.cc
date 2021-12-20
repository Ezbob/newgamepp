
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
    const nfdchar_t *current_directory = GetWorkingDirectory();
    nfdchar_t *path = NULL;
    const char *extensions = tileParser_->getFileExtentions();
    nfdresult_t result = NFD_OpenDialog(extensions, current_directory, &path);
    if (result == NFD_OKAY) {
      TraceLog(LOG_INFO, "Found path %s\n", path);
    }
  }

  if (!tileParser_) GuiEnable();

  return true;
}