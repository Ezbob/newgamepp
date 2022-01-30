
#pragma once

#include "entt/entity/registry.hpp"
#include "raylib.h"

namespace TilePicker {
  entt::entity findClickedTile(entt::registry &reg, Vector2 const& mouse, int layerIndex);
};
