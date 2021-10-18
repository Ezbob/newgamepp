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


float g_dt = 0.f;

void draw(entt::registry &reg) {
    auto view = reg.view<const position, const dimension>();

    view.each([](const auto &pos, const auto &dim) {
        DrawRectangle(pos.x, pos.y, dim.w, dim.h, WHITE);
    });
}

void update(entt::registry &reg) {
    auto view = reg.view<position, const velocity>();

    view.each([](auto &pos, const auto &vel) {
        pos.x += (int) (vel.dx * g_dt) ;
        pos.y += (int) (vel.dy * g_dt);
    });

}

float len(Vector2 &v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
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

class DebugGUI {

private:
    Rectangle r {
        10.f,
        200.f,
        200.f,
        110.f
    };

    Rectangle header {
        0.f,
        0.f,
        60.f,
        28.f
    };

    bool drawWindow = true;
    bool isDragging = false;

public:

    void render() {
        if (drawWindow) {
            if (GuiWindowBox(r, "Whello")) {
                drawWindow = false;
            }
        }

        if (IsKeyPressed('R')) {
            drawWindow = !drawWindow;
        }

        Vector2 mousepos = GetMousePosition();

        if (IsMouseButtonPressed(0)) {
            header.x = r.x;
            header.y = r.y;
            if (CheckCollisionPointRec(mousepos, header)) {
                isDragging = true;
            }
        }

        if (IsMouseButtonUp(0)) {
            isDragging = false;
        }

        if (IsMouseButtonDown(0)) {
            header.x = r.x;
            header.y = r.y;
            if (isDragging) {
                r.x += (mousepos.x - r.x) - (header.width / 2);
                r.y += (mousepos.y - r.y) - (header.height / 2);
            }
        }
    }
};



int main(void)
{
    entt::registry registry;
    DebugGUI dgui;

    const int screenWidth = 800;
    const int screenHeight = 450;

    {
        const auto entity = registry.create();
        registry.emplace<position>(entity, 10, 10);
        registry.emplace<dimension>(entity, 20, 20);
        registry.emplace<velocity>(entity, 100.f, 100.f);

        const auto entity2 = registry.create();
        registry.emplace<position>(entity2, screenWidth - 30,  screenHeight - 30);
        registry.emplace<dimension>(entity2, 20, 20);
        registry.emplace<velocity>(entity2, -100.f, -100.f);
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Raylib [raylib] example");

    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        g_dt = GetFrameTime();

        update(registry);

        dgui.render();

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
