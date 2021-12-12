
#include "TileWindow.hh"
#include "raygui.h"
#include "Constants.hh"
#include <stdint.h>

bool TileWindow::render() {
  if (GuiWindowBox(windowBoundary_, "Tile debugger")) {
    return false;
  }

  int spacing = 15;
  int xSpaces = (Constants::screenWidth / spacing);
  int ySpaces = (Constants::screenHeight / spacing);

  gridColor_ = GuiColorPicker({windowBoundary_.x + 5.f, windowBoundary_.y + 30.f, 150.f, 150.f}, gridColor_);

  for (int i = 1; i < ySpaces; ++i)
  {
    DrawLineEx(
      { 0, ((float) i) * spacing },
      { Constants::screenWidth, ((float) i) * spacing },
      1.f,
      gridColor_
    );
  }

  for (int i = 1; i < xSpaces; ++i)
  {
    DrawLineEx(
      { ((float) i) * spacing, 0 },
      { ((float) i) * spacing, Constants::screenHeight },
      1.f,
      gridColor_
    );
  }

  return true;
}