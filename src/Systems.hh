
#pragma once

#include "entt/entity/registry.hpp"
#include "Components.hh"
#include "Constants.hh"
#include "raylib.h"

void draw(entt::registry &reg) {
  auto view2 = reg.view<Components::TileTextures, Components::Tiles>();

  view2.each([](const Components::TileTextures &tiletextures, const Components::Tiles &tiles) {
    for (auto &t : tiles.tiles) {
      auto &texture = tiletextures.textures.at(*t.textureName);
      DrawTextureRec(texture, t.dimensions, t.position, ColorAlpha(WHITE, t.alpha));
    }
  });

  auto view = reg.view<const Components::Position, const Components::Dimensions>();

  view.each([](const auto &pos, const auto &dim) {
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