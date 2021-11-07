
#include "DebugGUI.hh"
#include "raylib.h"
#include "Constants.hh"

DebugGUI::DebugGUI(entt::registry &r) : entityChooser_(r) {}

void DebugGUI::doGui() {

  if (IsKeyPressed(KEY_F11)) {
    drawWindow_ = !drawWindow_;
    if (drawWindow_)
      SetWindowSize(Constants::debugScreenWidth, Constants::debugScreenHeight);
    else
      SetWindowSize(Constants::screenWidth, Constants::screenHeight);
  }
  if (drawWindow_) {
    if ( !entityChooser_.render() ) {
      drawWindow_ = false;
    }
  }
}
