
#pragma once

#define RAYGUIDEF extern "C"

#include <array>
#include <optional>

#include "SliderField.hh"
#include "entt/entity/registry.hpp"
#include "Components.hh"
#include "raylib.h"

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

    void drawEntity();
public:
    DebugGUI(entt::registry &);

    void doGui();
};

