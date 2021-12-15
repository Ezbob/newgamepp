
#include "TileWindow.hh"
#include "raygui.h"
#include "Constants.hh"
#include <stdint.h>
#include "nfd.h"

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

  int oldStyle = GuiGetStyle(DEFAULT, LINE_COLOR);
  GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(gridColor_));
  auto mouseGridPosition = GuiGrid({0, 0, Constants::screenWidth, Constants::screenHeight}, 10.f, 2);
  GuiSetStyle(DEFAULT, LINE_COLOR, oldStyle);

  auto tileBox = Rectangle{windowBoundary_.x + 5.f, windowBoundary_.height - 205.f, windowBoundary_.width - 10.f, 200.f};
  GuiGroupBox(tileBox, "Tilesets");

  if (GuiButton({tileBox.x + 10.f, tileBox.y + 10.f, 100.f, 30.f}, "Browse")) {
    nfdchar_t *path = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, NULL, &path);
    if (result == NFD_OKAY) {
      TraceLog(LOG_INFO, "Found path %s\n", path);
    }
  }

  return true;
}