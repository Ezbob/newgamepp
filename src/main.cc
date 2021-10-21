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
#include "SliderField.hh"
#include "DebugGUI.hh"

float g_dt = 0.f;

void draw(entt::registry &reg) {
    auto view = reg.view<const Position, const Dimensions>();

    view.each([](const auto &pos, const auto &dim) {
        DrawRectangle(pos.x, pos.y, dim.w, dim.h, WHITE);
    });
}

void update(entt::registry &reg) {
    auto view = reg.view<Position, const Velocity>();

    view.each([](auto &pos, const auto &vel) {
        pos.x += (int) (vel.dx * g_dt);
        pos.y += (int) (vel.dy * g_dt);
    });

}


float len(Vector2 &v) {
    return sqrtf(powf(v.x, 2) + powf(v.y, 2));
}

Vector2 norm(Vector2 &v) {
    static Vector2 zero{0.f, 0.f};
    float l = len(v);
    if (l <= 0) {
        return zero;
    }
    Vector2 result;
    result.x = v.x / l;
    result.y = v.y / l;
    return result;
}



int main(void)
{
    entt::registry registry;
    DebugGUI dgui{registry};

    const int screenWidth = 900;
    const int screenHeight = 650;

    {
        const auto entity = registry.create();
        registry.emplace<Position>(entity, 10, 10);
        registry.emplace<Dimensions>(entity, 20, 20);
        registry.emplace<Velocity>(entity, 0.f, 0.f);

        const auto entity2 = registry.create();
        registry.emplace<Position>(entity2, screenWidth - 30,  screenHeight - 30);
        registry.emplace<Dimensions>(entity2, 20, 20);
        registry.emplace<Velocity>(entity2, 0.f, 0.f);
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT);
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
