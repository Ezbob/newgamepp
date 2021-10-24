
#pragma once

#include <array>
#include <optional>

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

  Vector2 mousepos_;

  bool drawWindow_ = true;
  bool isDragging_ = false;
  bool mousepressed_ = false;

  entt::registry &registry_;

  std::array<SliderField, 2> sliderfields_ = {
      SliderField{windowBoundary_, Rectangle{230, 35, 120, 25}},
      SliderField{windowBoundary_, Rectangle{230, 35, 120, 25}}
  };

  TextField positionXField_{windowBoundary_, Rectangle{70, 35, 120, 25}};
  TextField positionYField_{windowBoundary_, Rectangle{70, 35, 120, 25}};

  TextField velocityXField_{windowBoundary_, Rectangle{70, 35, 120, 25}};
  TextField velocityYField_{windowBoundary_, Rectangle{70, 35, 120, 25}};

  void drawEntity(entt::entity &entity);

  //void findEntity();
public:

  EntityWindow(entt::registry &);

  bool doGui(entt::entity &entity);

};