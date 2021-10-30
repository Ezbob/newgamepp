
#pragma once

#include "entt/entity/registry.hpp"
#include "raylib.h"
#include <optional>
#include "EntityWindow.hh"
#include <vector>

class EntityChooser {

public:
  EntityChooser(entt::registry &);
  ~EntityChooser() = default;

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

  int selectedIndex_ = 0;
  bool drawWindow_ = false;
  bool mousepressed_ = false;
  bool isDragging_ = false;
  bool editable_ = false;

  EntityWindow entityWindow_;
  std::optional<entt::entity> selected_;
  std::vector<const char *> text_;

  enum class EntityMode {
    no_render,
    edit_mode
  } mode_ = EntityMode::no_render;

  Vector2 mousepos_;

  entt::registry &registry_;

  std::optional<entt::entity> findEntity(entt::registry &registry, Vector2 &mousePos);
};