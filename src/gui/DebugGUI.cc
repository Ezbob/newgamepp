
#include "DebugGUI.hh"
#include "Constants.hh"
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

DebugGUI::DebugGUI(entt::registry &r, Camera2D &cam) : entityChooser_(r, cam), camera_(cam) {}

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

  Vector2 currentMouse = GetMousePosition();
  Vector2 mouseWorld = GetScreenToWorld2D(currentMouse, camera_);
  if (IsMouseButtonDown(1)) {
    auto delta = Vector2Subtract(prevMouse_, currentMouse);
    camera_.target = Vector2Add(camera_.target, delta);
  }
  prevMouse_ = currentMouse;

  if (CheckCollisionPointRec(currentMouse, Constants::screenRectangle)) {
    camera_.zoom += ((float)GetMouseWheelMove() * 0.05f);
    camera_.zoom = Clamp(camera_.zoom, 0.5f, 1.5f);
  }

  GuiPanel({0, Constants::debugScreenHeight - 20.f, static_cast<float>(Constants::debugScreenWidth), 20.f});
  GuiValueBoxEx({ Constants::debugScreenWidth - 60.f, Constants::debugScreenHeight - 20.f, 40.f, 20.f}, "Zoom:", &camera_.zoom, 0.f, 10.f, false);

  GuiValueBoxEx({ Constants::debugScreenWidth - 180.f, Constants::debugScreenHeight - 20.f, 80.f, 20.f}, "WY:", &mouseWorld.y, 0.f, 10.f, false);
  GuiValueBoxEx({ Constants::debugScreenWidth - (145.f * 2.f), Constants::debugScreenHeight - 20.f, 80.f, 20.f}, "WX:", &mouseWorld.x, 0.f, 10.f, false);

  GuiValueBoxEx({ Constants::debugScreenWidth - (145.f * 3.f), Constants::debugScreenHeight - 20.f, 80.f, 20.f}, "SY:", &currentMouse.y, 0.f, 10.f, false);
  GuiValueBoxEx({ Constants::debugScreenWidth - (145.f * 3.f) - 110.f, Constants::debugScreenHeight - 20.f, 80.f, 20.f}, "SX:", &currentMouse.x, 0.f, 10.f, false);

}
