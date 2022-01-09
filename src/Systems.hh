
#pragma once

#include "entt/entity/registry.hpp"
#include "Components.hh"
#include "Constants.hh"
#include "raylib.h"

void draw(entt::registry &reg) {

  auto spriteGroup = reg.group<Components::Renderable>(
    entt::get<Components::SpriteTexture, Components::Position, Components::Quad>);

  spriteGroup.sort([&spriteGroup](entt::entity const& a, entt::entity const& b) {
    auto ar = spriteGroup.get<Components::Renderable>(a);
    auto br = spriteGroup.get<Components::Renderable>(b);
    return ar.zIndex < br.zIndex;
  });

  spriteGroup.each([&spriteGroup](Components::Renderable const &renderable,
    Components::SpriteTexture const &texture,
    Components::Position const &pos,
    Components::Quad const &dim) {
    DrawTextureRec(texture.texture, dim.quad, {pos.x, pos.y}, ColorAlpha(WHITE, renderable.alpha));
  });

  auto view = reg.view<const Components::Position, const Components::Dimensions>();

  view.each([](const Components::Position &pos, const Components::Dimensions &dim) {
    DrawRectangleRec({pos.x, pos.y, dim.w, dim.h}, WHITE);
  });

}

void update(entt::registry &reg, float dt) {
  {
    auto view = reg.view<Components::Position, const Components::Velocity>();

    view.each([dt](auto &pos, const auto &vel) {
      pos.x += (vel.dx * dt);
      pos.y += (vel.dy * dt);
    });
  }
  {
    // Clamping to screen boundaries with padding
    auto view = reg.view<Components::Position, const Components::Dimensions, const Components::ScreenClamp>();

    view.each([](Components::Position &pos, const Components::Dimensions &dim, const Components::ScreenClamp &screenCollision) {
      if (pos.x <= screenCollision.left) {
        pos.x = screenCollision.left;
      }

      if (pos.x + dim.w >= Constants::screenWidth - screenCollision.right) {
        pos.x = Constants::screenWidth - dim.w - screenCollision.right;
      }

      if (pos.y <= screenCollision.top) {
        pos.y = screenCollision.top;
      }

      if (pos.y + dim.h >= Constants::screenHeight - screenCollision.bottom) {
        pos.y = Constants::screenHeight - dim.h - screenCollision.bottom;
      }
    });
  }
}