#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "Components.hh"
#include "entt/entity/registry.hpp"
#include "gui/DebugGUI.hh"
#include "raygui.h"
#include "Constants.hh"


void draw(entt::registry &reg) {
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

int main() {
  const auto title = "Raylib example";

  entt::registry registry;
  DebugGUI dgui{registry};

  auto whiteBlockInitializer = [&registry](float xpos, float ypos, float width, float height, std::string name) {
    const auto entity = registry.create();
    registry.emplace<Components::Position>(entity, xpos, ypos);
    registry.emplace<Components::Dimensions>(entity, width, height);
    registry.emplace<Components::Velocity>(entity, 0.f, 0.f);
    registry.emplace<Components::Name>(entity, std::move(name));
    registry.emplace<Components::ScreenClamp>(entity);
  };

  whiteBlockInitializer(10.f, 10.f, 20.f, 20.f, "Hello");
  whiteBlockInitializer(Constants::screenWidth - 30.f, Constants::screenHeight - 30.f, 20.f, 20.f, "Yeet");

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  InitWindow((int) Constants::screenWidth,(int) Constants::screenHeight, title);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    update(registry, dt);

    BeginDrawing();

    ClearBackground(BLACK);

    draw(registry);
    dgui.doGui();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
