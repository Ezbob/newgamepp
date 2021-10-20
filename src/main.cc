#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include <stdio.h>
#include <stdbool.h>
#include "entt/entity/registry.hpp"
#include <cmath>
#include "raygui.h"
#include "fmt/core.h"
#include <optional>

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
        pos.x += (int) (vel.dx * g_dt);
        pos.y += (int) (vel.dy * g_dt);
    });

}

/*
void debug_velocity(entt::registry &reg, Rectangle &r) {
    auto view = reg.view<velocity>();

    size_t i = 1;

    view.each([&r, &i](auto &vel) {
        std::string s = fmt::format("{0}", vel.dx);

        GuiLabel({r.x + 200, r.y + (35 * i), 120, 25}, "Velocity: ");
        GuiTextBox({r.x + 245, r.y + (35 * i), 120, 25}, const_cast<char *>(s.c_str()), static_cast<int>(s.size()), false);

        float out = GuiSlider({r.x + 35, r.y + (35 * i), 120, 25}, "Min", "Max", vel.dx, -300.f, 300.f);
        vel.dx = out;
        vel.dy = out;

        i++;
    });
}
*/

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

class DebugGUI {

private:
    Rectangle r {
        10.f,
        200.f,
        400.f,
        400.f
    };

    Rectangle header {
        0.f,
        0.f,
        60.f,
        28.f
    };

    std::vector<Rectangle> children;

    bool drawWindow = true;
    bool isDragging = false;

    entt::registry &reg;

    std::optional<entt::entity> selected;


    void drawEntity() {
        if (selected) {
            auto entity = selected.value();

            size_t i = 1;
            if (reg.has<velocity>(entity)) {
                auto &vel = reg.get<velocity>(entity);

                std::string s = fmt::format("{0}", vel.dx);
                GuiLabel({r.x + 200, r.y + (35 * i), 120, 25}, "X Velocity: ");
                GuiTextBox({r.x + 265, r.y + (35 * i), 120, 25}, const_cast<char *>(s.c_str()), static_cast<int>(s.size()), false);
                float out = GuiSlider({r.x + 35, r.y + (35 * i), 120, 25}, "Min", "Max", vel.dx, -300.f, 300.f);
                vel.dx = out;
                i++;

                s = fmt::format("{0}", vel.dy);
                GuiLabel({r.x + 200, r.y + (35 * i), 120, 25}, "Y Velocity: ");
                GuiTextBox({r.x + 265, r.y + (35 * i), 120, 25}, const_cast<char *>(s.c_str()), static_cast<int>(s.size()), false);
                out = GuiSlider({r.x + 35, r.y + (35 * i), 120, 25}, "Min", "Max", vel.dy, -300.f, 300.f);
                vel.dy = out;
                i++;
            }
        }
    }

public:

    DebugGUI (entt::registry &r) : reg(r) {}

    void doGui() {
        Vector2 mousepos = GetMousePosition();

        if (!selected.has_value()) {
            auto view = reg.view<position, dimension>();

            for (auto ent : view) {
                auto const &pos = view.get<position>(ent);
                auto const &dim = view.get<dimension>(ent);
                Rectangle entityBox {
                    pos.x,
                    pos.y,
                    dim.w,
                    dim.h
                };
                if (IsMouseButtonPressed(0) && CheckCollisionPointRec(mousepos, entityBox)) {
                    selected = ent;
                    break;
                }
            }
        }

        if (drawWindow && selected.has_value()) {
            if (GuiWindowBox(r, "Debug - selected")) {
                selected = std::nullopt;
            } else {
                drawEntity();
            }
        }

        if (IsKeyPressed('R') && selected.has_value()) {
            drawWindow = !drawWindow;
        }

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
    DebugGUI dgui{registry};

    const int screenWidth = 900;
    const int screenHeight = 650;

    {
        const auto entity = registry.create();
        registry.emplace<position>(entity, 10, 10);
        registry.emplace<dimension>(entity, 20, 20);
        registry.emplace<velocity>(entity, 0.f, 0.f);

        const auto entity2 = registry.create();
        registry.emplace<position>(entity2, screenWidth - 30,  screenHeight - 30);
        registry.emplace<dimension>(entity2, 20, 20);
        registry.emplace<velocity>(entity2, 0.f, 0.f);
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

        EndDrawing();

        dgui.doGui();
    }

    CloseWindow();
    return 0;
}
