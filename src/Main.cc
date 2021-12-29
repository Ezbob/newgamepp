
#include "raylib.h"
#include "Components.hh"
#include "Constants.hh"
#include "entt/entity/registry.hpp"
#include "gui/DebugGUI.hh"
#include "Systems.hh"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION


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

  auto tilelayerInitializer = [&registry]() {
    const auto entity = registry.create();
    registry.emplace<Components::TileTexture>(entity);
    registry.emplace<Components::Tiles>(entity);
  };

  tilelayerInitializer();

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  InitWindow((int) (Constants::screenWidth), (int) (Constants::screenHeight), title);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    update(registry, dt);

    BeginDrawing();

    ClearBackground(BLACK);

    dgui.doGui();

    draw(registry);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
