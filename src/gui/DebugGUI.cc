
#include "DebugGUI.hh"
#include "Constants.hh"
#include "raylib.h"

DebugGUI::DebugGUI(entt::registry &r) : entityChooser_(r) {}

void DebugGUI::doGui() {

  if (IsKeyPressed(KEY_F11)) {
    drawWindow_ = !drawWindow_;
    if (drawWindow_) {
      SetWindowSize(
        static_cast<int>(Constants::debugScreenWidth),
        static_cast<int>(Constants::debugScreenHeight)
      );
    } else {
      SetWindowSize(
        static_cast<int>(Constants::screenWidth),
        static_cast<int>(Constants::screenHeight)
      );
    }
  }
  if (drawWindow_) {
    if (!entityChooser_.render()) {
      drawWindow_ = false;
    }
  }
}
