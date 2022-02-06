
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
  selectionBox_ = selectModel_.create(reg);
}

void MultiSelectTool::execute() {
  auto mousePosition = GetMousePosition();
  auto world = GetScreenToWorld2D(mousePosition, camera_);

  if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {

    if (IsMouseButtonPressed(0)) {
      selectModel_.boundary.x = world.x;
      selectModel_.boundary.y = world.y;
      selectModel_.show = true;
    }

    if (selectModel_.show && IsMouseButtonDown(0)) {
      selectModel_.boundary.width = (world.x - selectModel_.boundary.x);
      selectModel_.boundary.height = (world.y - selectModel_.boundary.y);
    }

    if (selectModel_.show && IsMouseButtonUp(0)) {
      selectModel_.boundary.width = (world.x - selectModel_.boundary.x);
      selectModel_.boundary.height = (world.y - selectModel_.boundary.y);
      selectModel_.show = false;

      auto spriteGroup = registry_.group<Components::Renderable>(
          entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>);


      auto entities = spriteGroup | std::views::filter([&spriteGroup, this](entt::entity entity) {
        auto render = spriteGroup.get<Components::Renderable>(entity);
        auto position = spriteGroup.get<Components::Position>(entity);
        auto quad = spriteGroup.get<Components::Quad>(entity);


        Rectangle spriteBox = {position.x, position.y, quad.quad.width, quad.quad.height};
        bool is_colliding = CheckCollisionRecs(spriteBox, selectModel_.boundary);

        return render.layer == currentLayer_ && is_colliding;
      });

      selections_.clear();
      for (entt::entity e : entities) {
        selections_.push_back(e);
      }

    }

  }

  selectModel_.update(registry_, selectionBox_);
}
