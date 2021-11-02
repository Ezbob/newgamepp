
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

  Vector2 mousepos_ = {0.f, 0.f};

  int selectedIndex_ = 0;
  std::vector<const char *> text_;
  bool editable_ = false;
  bool isDragging_ = false;
  bool mousepressed_ = false;

  entt::registry &registry_;
  std::optional<entt::entity> selected_;

  TextField nameField_{windowBoundary_, {70, 35, 120, 25}};

  TextField positionXField_{windowBoundary_, {70, 35, 120, 25}};
  TextField positionYField_{windowBoundary_, {70, 35, 120, 25}};

  TextField velocityXField_{windowBoundary_, {70, 37, 120, 25}};
  TextField velocityYField_{windowBoundary_, {70, 37, 120, 25}};
  SliderField sliderVelocityXField_{windowBoundary_, {230, 37, 120, 25}};
  SliderField sliderVelocityYField_{windowBoundary_, {230, 37, 120, 25}};

  std::optional<entt::entity> findEntity(entt::registry &registry, Vector2 &mousePos) const;
  void drawEntity();

public:

  explicit EntityWindow(entt::registry &);

  bool render();

};