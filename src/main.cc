#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "entt/entity/registry.hpp"
#include "raygui.h"
#include "Components.hh"
#include "gui/DebugGUI.hh"

float g_dt = 0.f;

void draw(entt::registry &reg) {
    auto view = reg.view<const Components::Position, const Components::Dimensions>();

    view.each([](const auto &pos, const auto &dim) {
        DrawRectangleRec({pos.x, pos.y, dim.w, dim.h}, WHITE);
    });
}

void update(entt::registry &reg) {
    auto view = reg.view<Components::Position, const Components::Velocity>();

    view.each([](auto &pos, const auto &vel) {
        pos.x += (vel.dx * g_dt);
        pos.y += (vel.dy * g_dt);
    });
}

int main() {

    entt::registry registry;
    DebugGUI dgui{registry};

    const int screenWidth = 900;
    const int screenHeight = 650;

    {
        const auto entity = registry.create();
        registry.emplace<Components::Position>(entity, 10.f, 10.f);
        registry.emplace<Components::Dimensions>(entity, 20.f, 20.f);
        registry.emplace<Components::Velocity>(entity, 0.f, 0.f);
        registry.emplace<Components::Name>(entity, "Hello");

        const auto entity2 = registry.create();
        registry.emplace<Components::Position>(entity2, (float)(screenWidth - 30.f),  (float)(screenHeight - 30.f));
        registry.emplace<Components::Dimensions>(entity2, 20.f, 20.f);
        registry.emplace<Components::Velocity>(entity2, 0.f, 0.f);
        registry.emplace<Components::Name>(entity2, "Yeet");
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Raylib [raylib] example");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        g_dt = GetFrameTime();

        update(registry);


        BeginDrawing();

        ClearBackground(BLACK);

        draw(registry);
        dgui.doGui();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
