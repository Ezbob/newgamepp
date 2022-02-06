
#include "MultiSelectTool.hh"
#include "TilePicker.hh"
#include "raylib.h"
#include "raygui.h"
#include "Constants.hh"
#include "TilePicker.hh"
#include <ranges>
#include "entt/entity/registry.hpp"
#include "Components.hh"

namespace {
  void getCorners(Rectangle r, Vector2 &upper, Vector2 &lower) {
    upper.x = r.x;
    upper.y = r.y;
    lower.x = r.x + r.width;
    lower.y = r.y + r.height;
  }

  void mergeCorners(Vector2 upper, Vector2 lower, Rectangle &r) {
    r.x = upper.x;
    r.y = upper.y;
    r.width = abs(lower.x - upper.x);
    r.height = abs(lower.y - upper.y);
  }
};

MultiSelectTool::MultiSelectTool(entt::registry &reg, int &layer, std::vector<entt::entity> &sel, Camera2D &cam)
  : registry_(reg)
  , currentLayer_(layer) 
  , selections_(sel)
  , camera_(cam)
{
  selectionBox_ = reg.create();
  reg.emplace<Components::Quad>(selectionBox_);
  reg.emplace<Components::Active>(selectionBox_);
  reg.emplace<Components::Debug>(selectionBox_);
}

void MultiSelectTool::execute() {
  auto mousePosition = GetMousePosition();
  auto world = GetScreenToWorld2D(mousePosition, camera_);

  if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {
    auto &active_comp = registry_.get<Components::Active>(selectionBox_);


    if (IsMouseButtonPressed(0)) {
      auto &boundaries = registry_.get<Components::Quad>(selectionBox_);
      boundaries.quad.x = world.x;
      boundaries.quad.y = world.y;
      active_comp.isActive = true;
    }

    if (active_comp.isActive && IsMouseButtonDown(0)) {
      auto &boundaries = registry_.get<Components::Quad>(selectionBox_);
      boundaries.quad.width = (world.x - boundaries.quad.x);
      boundaries.quad.height = (world.y - boundaries.quad.y);
    }

    if (active_comp.isActive && IsMouseButtonUp(0)) {
      auto &boundaries = registry_.get<Components::Quad>(selectionBox_);
      boundaries.quad.width = (world.x - boundaries.quad.x);
      boundaries.quad.height = (world.y - boundaries.quad.y);
      active_comp.isActive = false;

      for (entt::entity et : selections_) {
        if (registry_.valid(et) && registry_.all_of<Components::Debug>(et)) {
          registry_.remove<Components::Debug>(et);
        }
      }

      selections_.clear();

      auto spriteGroup = registry_.group<Components::Renderable>(
          entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>);

      for (entt::entity entity : spriteGroup) {
        auto render = spriteGroup.get<Components::Renderable>(entity);
        auto position = spriteGroup.get<Components::Position>(entity);
        auto quad = spriteGroup.get<Components::Quad>(entity);

        Rectangle spriteBox = {position.x, position.y, quad.quad.width, quad.quad.height};
        bool is_colliding = CheckCollisionRecs(spriteBox, boundaries.quad);

        if (render.layer == currentLayer_ && is_colliding) {
          registry_.emplace<Components::Debug>(entity);
          selections_.push_back(entity);
        }
      }
    }

  }
}
