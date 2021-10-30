
#include "DebugGUI.hh"
#include "raylib.h"

DebugGUI::DebugGUI(entt::registry &r) : entityChooser_(r) {}

void DebugGUI::doGui() {

  if (IsKeyPressed('R')) {
    drawWindow_ = !drawWindow_;
  }
  if (drawWindow_) {
    if ( !entityChooser_.render() ) {
      drawWindow_ = false;
    }
  }
}
