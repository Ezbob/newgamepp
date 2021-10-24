#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include <stdio.h>
#include <stdbool.h>
#include "entt/entity/registry.hpp"
#include <cmath>
#include "raygui.h"
#include "fmt/core.h"
#include <optional>
#include <array>
#include "Components.hh"
#include "gui/DebugGUI.hh"

float g_dt = 0.f;

void draw(entt::registry &reg) {
    auto view = reg.view<const Position, const Dimensions>();

    view.each([](const auto &pos, const auto &dim) {
        DrawRectangleRec({pos.x, pos.y, dim.w, dim.h}, WHITE);
    });
}

void update(entt::registry &reg) {
    auto view = reg.view<Position, const Velocity>();

    view.each([](auto &pos, const auto &vel) {
        pos.x += (vel.dx * g_dt);
        pos.y += (vel.dy * g_dt);
    });
}

int main(void) {

    entt::registry registry;
    DebugGUI dgui{registry};

    const int screenWidth = 900;
    const int screenHeight = 650;

    {
        const auto entity = registry.create();
        registry.emplace<Position>(entity, 10.f, 10.f);
        registry.emplace<Dimensions>(entity, 20.f, 20.f);
        registry.emplace<Velocity>(entity, 0.f, 0.f);
        registry.emplace<Name>(entity, "Hello");

        const auto entity2 = registry.create();
        registry.emplace<Position>(entity2, (float)(screenWidth - 30.f),  (float)(screenHeight - 30.f));
        registry.emplace<Dimensions>(entity2, 20.f, 20.f);
        registry.emplace<Velocity>(entity2, 0.f, 0.f);
        registry.emplace<Name>(entity2, "Yeet");
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
