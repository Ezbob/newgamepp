
#pragma once

#include "entt/entity/registry.hpp"
#include "raylib.h"
#include <optional>
#include "EntityWindow.hh"
#include <vector>

class DebugMenu {

public:
  explicit DebugMenu(entt::registry &);
  ~DebugMenu() = default;

  bool render();

private:
  Rectangle windowBoundary_ {
    0.f,
    Constants::screenHeight,
    200.f,
    200.f
  };

  bool mousepressed_ = false;

  EntityWindow entityWindow_;

  enum class EntityMode {
    no_render,
    entity_edit_mode
  } mode_ = EntityMode::no_render;

  Vector2 mousepos_ = {};

};