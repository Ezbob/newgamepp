
#include "DebugGUI.hh"

#include "raygui.h"

DebugGUI::DebugGUI (entt::registry &r) : registry_(r) {}

void DebugGUI::drawEntity() {
    if (selected_) {
        auto entity = selected_.value();

        size_t i = 0;
        if (registry_.has<Velocity>(entity)) {
            auto &vel = registry_.get<Velocity>(entity);

            sliderfields_[i++].render(i, mousepressed_, mousepos_, vel.dx);
            sliderfields_[i++].render(i, mousepressed_, mousepos_, vel.dy);
        }
    }
}

void DebugGUI::doGui() {
    mousepos_ = GetMousePosition();
    mousepressed_ = IsMouseButtonPressed(0);

    if (mousepressed_) {
        auto view = registry_.view<Position, Dimensions>();

        for (auto ent : view) {
            auto const &pos = view.get<Position>(ent);
            auto const &dim = view.get<Dimensions>(ent);
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