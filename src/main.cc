#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include <stdio.h>
#include <stdbool.h>
#include "entt/entity/registry.hpp"
#include <cmath>
#include "raygui.h"

struct position {
    int x;
    int y;
};

struct dimension {
    int w;
    int h;
};

struct velocity {
    float dx;
    float dy;
};

void draw(entt::registry &reg) {
    auto view = reg.view<const position, const dimension>();

    view.each([](const auto &pos, const auto &dim) {
        DrawRectangle(pos.x, pos.y, dim.w, dim.h, WHITE);
    });
}

void update(entt::registry &reg) {
    auto view = reg.view<position, const velocity>();

    float dt = GetFrameTime();
    view.each([dt](auto &pos, const auto &vel) {
        pos.x += (int) (vel.dx * dt) ;
        pos.y += (int) (vel.dy * dt);
    });

}

int main(void)
{
    entt::registry registry;

    const int screenWidth = 800;
    const int screenHeight = 450;

    {
        const auto entity = registry.create();
        registry.emplace<position>(entity, 10, 10);
        registry.emplace<dimension>(entity, 20, 20);
        registry.emplace<velocity>(entity, 100.f, 100.f);

        const auto entity2 = registry.create();
        registry.emplace<position>(entity2, screenWidth - 10,  screenHeight - 10);
        registry.emplace<dimension>(entity2, 20, 20);
        registry.emplace<velocity>(entity2, -100.f, -100.f);
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Raylib [raylib] example");

    SetTargetFPS(60);

    Rectangle r {
        10,
        200,
        200,
        110
    };

    bool drawWindow = true;

    while (!WindowShouldClose())
    {

        update(registry);

        if (drawWindow) {
            if (GuiWindowBox(r, "Whello")) {
                drawWindow = false;
            }
        }
        if (IsKeyPressed('r')) {
            drawWindow = true;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        draw(registry);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
