
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

  Camera2D cam = {0};
  cam.zoom = 1.f;

  DebugGUI dgui(registry, cam);

  auto whiteBlockInitializer = [&registry](float xpos, float ypos, float width, float height, std::string_view const name) {
    const auto entity = registry.create();
    registry.emplace<Components::Position>(entity, xpos, ypos);
    registry.emplace<Components::Dimensions>(entity, width, height);
    registry.emplace<Components::Velocity>(entity, 0.f, 0.f);
    registry.emplace<Components::Name>(entity, std::string(name));
  };

  whiteBlockInitializer(10.f, 10.f, 20.f, 20.f, "Hello");
  whiteBlockInitializer(Constants::screenWidth - 30.f, Constants::screenHeight - 30.f, 20.f, 20.f, "Yeet");


  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow((int) (Constants::screenWidth), (int) (Constants::screenHeight), title);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    Systems::update(registry, dt);

    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode2D(cam);

    Systems::draw(registry, cam);

    EndMode2D();

    dgui.doGui();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
