
#pragma once

#include "entt/entity/registry.hpp"
#include "raylib.h"
#include <optional>
#include "EntityWindow.hh"
#include <vector>
#include "DebugTileEditor.hh"

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

  EntityWindow entityWindow_;
  DebugTileEditor tileWindow_;

  enum class DebugWindowChoice {
    no_window,
    entity_window,
    tile_window
  } window_choice_ = DebugWindowChoice::entity_window;

  bool entityWindowEnabled_ = true;
};