
#include "RemoveTool.hh"
#include "raylib.h"
#include "TilePicker.hh"
#include "Constants.hh"
#include "Components.hh"

RemoveTool::RemoveTool(entt::registry &reg, Camera2D &camera, int &currentLayerId, std::vector<entt::entity> &selected)
  : registry_(reg)
  , camera_(camera)
  , currentLayerId_(currentLayerId)
  , selected_(selected)
{
}

void RemoveTool::execute() {
  if (IsMouseButtonPressed(0)) {
    auto mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {
      mousePosition = GetScreenToWorld2D(mousePosition, camera_);

      auto entity = TilePicker::findClickedTile(registry_, mousePosition, currentLayerId_);
      if (registry_.all_of<Components::Debug>(entity)) {
        for (std::vector<entt::entity>::iterator it = selected_.begin(); it != selected_.end(); it++) {
          if (*it == entity) {
            selected_.erase(it);
            break;
          }
        }
      }
      
      if (registry_.valid(entity)) {
        registry_.destroy(entity);
      }
    }
  }
}