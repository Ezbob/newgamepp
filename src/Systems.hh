
#pragma once

#include "entt/entity/registry.hpp"
#include "Components.hh"
#include "Constants.hh"
#include "raylib.h"
#include "raygui.h"

namespace {
  void drawCornerBox(Rectangle bounds, float thick = 1.f, float lineLength = 10.f, Color color = GREEN) {
    auto endX = bounds.x + bounds.width;
    auto endY = bounds.y + bounds.height;

    // upper left corner
    DrawLineEx({bounds.x, bounds.y}, {bounds.x + lineLength, bounds.y}, thick, color);// horizontal
    DrawLineEx({bounds.x, bounds.y}, {bounds.x, bounds.y + lineLength}, thick, color);// vertical

    // upper right corner
    DrawLineEx({endX, bounds.y}, {endX - lineLength, bounds.y}, thick, color);
    DrawLineEx({endX, bounds.y}, {endX, bounds.y + lineLength}, thick, color);

    // lower left corner
    DrawLineEx({bounds.x, endY}, {bounds.x + lineLength, endY}, thick, color);
    DrawLineEx({bounds.x, endY}, {bounds.x, endY - lineLength}, thick, color);

    // lower right corner
    DrawLineEx({endX, endY}, {endX - lineLength, endY}, thick, color);
    DrawLineEx({endX, endY}, {endX, endY - lineLength}, thick, color);
  }
}

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
        const Components::Dimensions>();

    for(auto [debug, active, coloring, dimension]: debugGridView.each()) {
      if (active.isActive) {

        float size = dimension.w;

        for (float i = -size; i <= size; i += 10.f)
        {
            DrawLineEx({i, -size}, {i, size}, 1.f, Fade(coloring.color, 0.8f));
            DrawLineEx({-size, i}, {size, i}, 1.f, Fade(coloring.color, 0.8f));
        }

      }
    }



    spriteGroup.each([&spriteGroup, &cam](Components::Renderable const &renderable,
      Components::SpriteTexture const &texture,
      Components::Position const &pos,
      Components::Quad const &dim,
      Components::Flipable const &flip) {
      DrawTextureRec(texture.texture, {
        dim.quad.x,
        dim.quad.y,
        flip.vFlipped ? -dim.quad.width : dim.quad.width,
        flip.hFlipped ? -dim.quad.height : dim.quad.height
      }, {pos.x, pos.y}, ColorAlpha(WHITE, renderable.alpha));
    });

    auto selection = reg.view<const Components::Debug,
        const Components::Position,
        const Components::Quad>();

    for(auto [debug, position, dimension]: selection.each()) {
      drawCornerBox({position.x, position.y, dimension.quad.width, dimension.quad.height});
    }

    auto view = reg.view<const Components::Position, const Components::Dimensions>(entt::exclude<Components::Debug>);

    view.each([&cam](const Components::Position &pos, const Components::Dimensions &dim) {
      DrawRectangleRec({pos.x, pos.y, dim.w, dim.h}, WHITE);
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