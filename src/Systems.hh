
#pragma once

#include "entt/entity/registry.hpp"
#include "Components.hh"
#include "Constants.hh"
#include "raylib.h"
#include "raygui.h"

namespace Systems {

  inline void draw(entt::registry &reg, Camera2D &cam) {

    auto spriteGroup = reg.group<Components::Renderable>(
      entt::get<Components::SpriteTexture, Components::Position, Components::Quad, Components::Flipable>
    );

    spriteGroup.sort([&spriteGroup](entt::entity const& a, entt::entity const& b) {
      auto az = spriteGroup.get<Components::Renderable>(a);
      auto bz = spriteGroup.get<Components::Renderable>(b);

      if (az.zIndex == bz.zIndex) {
        return az.layer < bz.layer;
      } else {
        return az.zIndex < bz.zIndex;
      }
    });


    auto debugGridView = reg.view<const Components::Debug,
        const Components::Active,
        const Components::Coloring,
        const Components::Position,
        const Components::Dimensions>();

    for(auto [debug, active, coloring, position, dimension]: debugGridView.each()) {
      if (active.isActive) {
        auto world = GetScreenToWorld2D({position.x, position.y}, cam);
        int oldStyle = GuiGetStyle(DEFAULT, LINE_COLOR);
        GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(coloring.color));
        GuiGrid({
          world.x, world.y, dimension.w, dimension.h
        }, 10.f, 2);
        GuiSetStyle(DEFAULT, LINE_COLOR, oldStyle);
      }
    }

    spriteGroup.each([&spriteGroup, &cam](Components::Renderable const &renderable,
      Components::SpriteTexture const &texture,
      Components::Position const &pos,
      Components::Quad const &dim,
      Components::Flipable const &flip) {
      auto world = GetScreenToWorld2D({dim.quad.x, dim.quad.y}, cam);
      DrawTextureRec(texture.texture, {
        world.x,
        world.y,
        flip.vFlipped ? -dim.quad.width : dim.quad.width,
        flip.hFlipped ? -dim.quad.height : dim.quad.height
      }, {pos.x, pos.y}, ColorAlpha(WHITE, renderable.alpha));
    });

    auto view = reg.view<const Components::Position, const Components::Dimensions>(entt::exclude<Components::Debug>);

    view.each([&cam](const Components::Position &pos, const Components::Dimensions &dim) {
      auto world = GetScreenToWorld2D({pos.x, pos.y}, cam);
      DrawRectangleRec({world.x, world.y, dim.w, dim.h}, WHITE);
    });

  }

  inline void update(entt::registry &reg, float dt) {
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
}