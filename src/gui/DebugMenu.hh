
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
    50.f,
    200.f,
    300.f,
    200.f
  };

  Rectangle header_ {
    0.f,
    0.f,
    90.f,
    28.f
  };

  bool mousepressed_ = false;
  bool isDragging_ = false;

  EntityWindow entityWindow_;

  enum class EntityMode {
    no_render,
    entity_edit_mode
  } mode_ = EntityMode::no_render;

  Vector2 mousepos_ = {};

};