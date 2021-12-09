
#include "TileWindow.hh"
#include "raygui.h"

bool TileWindow::render() {
  if (GuiWindowBox(windowBoundary_, "Tile debugger")) {
    return false;
  }

  return true;
}