
#pragma once

#include <array>
#include <optional>

#define RAYGUIDEF extern "C"
#include "raylib.h"
#include "fields/SliderField.hh"
#include "fields/TextField.hh"
#include "entt/entity/registry.hpp"
#include "Components.hh"


class EntityWindow {
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
      SliderField{windowBoundary_},
      SliderField{windowBoundary_}
  };

  TextField positionXField_ = TextField{windowBoundary_, Rectangle{70, 35, 120, 25}};
  TextField positionYField_ = TextField{windowBoundary_, Rectangle{70, 35, 120, 25}};

  void drawEntity();
public:

  EntityWindow(entt::registry &);

  void doGui();

};