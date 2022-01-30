
#include "TilePicker.hh"
#include "raylib.h"
#include "Constants.hh"
#include "Components.hh"

entt::entity TilePicker::findClickedTile(entt::registry &reg, Vector2 const& mouse, int layerIndex) {
  auto spriteGroup = reg.group<Components::Renderable>(
          entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>);

  auto it = std::find_if(spriteGroup.rbegin(), spriteGroup.rend(),
    [&mouse, &spriteGroup, layerIndex](entt::entity entity) {
      auto sprite = spriteGroup.get<Components::SpriteTexture>(entity);
      auto render = spriteGroup.get<Components::Renderable>(entity);
      auto position = spriteGroup.get<Components::Position>(entity);
      auto quad = spriteGroup.get<Components::Quad>(entity);

      Rectangle boundingBox = {position.x, position.y, quad.quad.width, quad.quad.height};

      bool is_colliding = CheckCollisionPointRec(mouse, boundingBox);

      return layerIndex == render.layer && is_colliding;
    });

  if (it != spriteGroup.rend()) {
    return *it;
  }

  return entt::null;
}
