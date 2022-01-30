
#include "RemoveTool.hh"
#include "raylib.h"
#include "TilePicker.hh"
#include "Constants.hh"

RemoveTool::RemoveTool(entt::registry &reg, Camera2D &camera, int &currentLayerId)
  : registry_(reg)
  , camera_(camera)
  , currentLayerId_(currentLayerId)
{
}

void RemoveTool::execute() {
  if (IsMouseButtonPressed(0)) {
    auto mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {
      mousePosition = GetScreenToWorld2D(mousePosition, camera_);

      auto entity = TilePicker::findClickedTile(registry_, mousePosition, currentLayerId_);
      if (registry_.valid(entity)) {
        registry_.destroy(entity);
      }
    }
  }
}