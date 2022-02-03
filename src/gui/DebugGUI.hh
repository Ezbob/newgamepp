
#pragma once

#include "Components.hh"
#include "DebugMenu.hh"
#include "entt/entity/registry.hpp"
#include <optional>

class DebugGUI {

private:
  Camera2D &camera_;
  DebugMenu entityChooser_;
  Vector2 prevMouse_;
  bool drawWindow_ = false;

public:
  explicit DebugGUI(entt::registry &, Camera2D &);

  void doGui();
};
