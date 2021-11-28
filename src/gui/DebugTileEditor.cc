
#include "DebugTileEditor.hh"
#include "Constants.hh"
#include "raylib.h"

DebugTileEditor::DebugTileEditor() {

}

bool DebugTileEditor::render() {

    DrawLine(0, 0, Constants::screenWidth, Constants::screenHeight, WHITE);
}