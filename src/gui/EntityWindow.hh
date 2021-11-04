
#pragma once

#include <array>
#include <optional>

#include "raylib.h"
#include "fields/SliderField.hh"
#include "fields/TextField.hh"
#include "entt/entity/registry.hpp"
#include "Components.hh"
#include "Constants.hh"

class EntityWindow {
private:
  Rectangle windowBoundary_ {
    Constants::screenWidth,
    0.f,
    600.f,
    400.f
  };

  Vector2 mousepos_ = {0.f, 0.f};

  int selectedIndex_ = 0;
  std::vector<const char *> text_;
  bool editable_ = false;
  bool mousepressed_ = false;

  entt::registry &registry_;
  std::optional<entt::entity> selected_;

  bool nameFieldEditable_ = false;

  bool velocityXFieldEditable_ = false;
  bool velocityYFieldEditable_ = false;

  bool positionXFieldEditable_ = false;
  bool positionYFieldEditable_ = false;

  void drawEntity();

public:

  explicit EntityWindow(entt::registry &);

  bool render();

};