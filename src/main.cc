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

#include "SliderField.hh"

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
    Rectangle windowBoundary_ {
        10.f,
        200.f,
        400.f,
        400.f
    };

    Rectangle header_ {
        0.f,
        0.f,
        90.f,
        28.f
    };

    bool drawWindow_ = true;
    bool isDragging_ = false;
    Vector2 mousepos_;
    bool mousepressed_ = false;

    entt::registry &registry_;

    std::optional<entt::entity> selected_;

    std::array<SliderField, 2> sliderfields_ = {
        SliderField{windowBoundary_, "Velocity X: "},
        SliderField{windowBoundary_, "Velocity Y: "}
    };


    void drawEntity() {
        if (selected_) {
            auto entity = selected_.value();

            size_t i = 0;
            if (registry_.has<velocity>(entity)) {
                auto &vel = registry_.get<velocity>(entity);

                sliderfields_[i++].render(i, mousepressed_, mousepos_, vel.dx);
                sliderfields_[i++].render(i, mousepressed_, mousepos_, vel.dy);
            }
        }
    }

public:

    DebugGUI (entt::registry &r) : registry_(r) {}

    void doGui() {
        mousepos_ = GetMousePosition();
        mousepressed_ = IsMouseButtonPressed(0);

        if (mousepressed_) {
            auto view = registry_.view<position, dimension>();

            for (auto ent : view) {
                auto const &pos = view.get<position>(ent);
                auto const &dim = view.get<dimension>(ent);
                Rectangle entityBox {
                    (float)(pos.x),
                    (float)(pos.y),
                    (float)(dim.w),
                    (float)(dim.h)
                };
                if (CheckCollisionPointRec(mousepos_, entityBox)) {
                    selected_ = ent;
                    break;
                }
            }
        }

        if (drawWindow_ && selected_.has_value()) {
            if (GuiWindowBox(windowBoundary_, "Entity Selected")) {
                selected_ = std::nullopt;
            } else {
                drawEntity();
            }
        }

        if (IsKeyPressed('R') && selected_.has_value()) {
            drawWindow_ = !drawWindow_;
        }

        if (mousepressed_) {
            header_.x = windowBoundary_.x;
            header_.y = windowBoundary_.y;
            if (CheckCollisionPointRec(mousepos_, header_)) {
                isDragging_ = true;
            }
        }

        if (IsMouseButtonUp(0)) {
            isDragging_ = false;
        }

        if (IsMouseButtonDown(0)) {
            header_.x = windowBoundary_.x;
            header_.y = windowBoundary_.y;
            if (isDragging_) {
                windowBoundary_.x += (mousepos_.x - windowBoundary_.x) - (header_.width / 2);
                windowBoundary_.y += (mousepos_.y - windowBoundary_.y) - (header_.height / 2);
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

        dgui.doGui();

        BeginDrawing();

        ClearBackground(BLACK);

        draw(registry);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
