
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
  /**
   * Handle "Signed" rectangles, i.e. rectangles with potential negative
   * width and height. This function computes the "upper" point of the rectangle 
   * if the width or height are negative so that, the invariant that a rectangle is
   * a point + a positive offset holds.
   */
  Rectangle flipSignedRectangle(Rectangle const &r) {
    Rectangle boundaries(r);
    if (r.width < 0) {
      auto xs = r.x + r.width;
      boundaries.x = xs;
      boundaries.width = abs(r.width);
    }

    if (r.height < 0) {
      auto ys = r.y + r.height;
      boundaries.y = ys;
      boundaries.height = abs(r.height);
    }
    return boundaries;
  }
};

MultiSelectTool::MultiSelectTool(entt::registry &reg, int &layer, SelectedCollection &sel, Camera2D &cam)
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

      selections_.clear();

      auto spriteGroup = registry_.group<Components::Renderable>(
          entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>);

      Rectangle flippedBoundaries = flipSignedRectangle(boundaries.quad);

      for (entt::entity entity : spriteGroup) {
        auto render = spriteGroup.get<Components::Renderable>(entity);
        auto position = spriteGroup.get<Components::Position>(entity);
        auto quad = spriteGroup.get<Components::Quad>(entity);

        Rectangle spriteBox = {position.x, position.y, quad.quad.width, quad.quad.height};
        bool is_colliding = CheckCollisionRecs(spriteBox, flippedBoundaries);

        if (render.layer == currentLayer_ && is_colliding) {
          selections_.push_back(entity);
        }
      }
    }

  }
}
