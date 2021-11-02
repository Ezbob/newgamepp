
#pragma once

#include "Components.hh"
#include "DebugMenu.hh"
#include "entt/entity/registry.hpp"
#include <optional>

class DebugGUI {

private:
  DebugMenu entityChooser_;
  bool drawWindow_ = false;

public:
  explicit DebugGUI(entt::registry &);

  void doGui();
};
