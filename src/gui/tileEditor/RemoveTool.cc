
#include "RemoveTool.hh"
#include "raylib.h"
#include "TilePicker.hh"
#include "Constants.hh"
#include "Components.hh"

RemoveTool::RemoveTool(entt::registry &reg, Camera2D &camera, int &currentLayerId, SelectedCollection &selected)
  : registry_(reg)
  , camera_(camera)
  , currentLayerId_(currentLayerId)
  , selected_(selected)
{
}

void RemoveTool::execute() {
  if (IsMouseButtonPressed(0)) {
    auto mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, Constants::screenRectangle)) {
      mousePosition = GetScreenToWorld2D(mousePosition, camera_);

      auto entity = TilePicker::findClickedTile(registry_, mousePosition, currentLayerId_);
      if (registry_.valid(entity)) {
        selected_.remove(entity);
        registry_.destroy(entity);
      }
    }
  }
}