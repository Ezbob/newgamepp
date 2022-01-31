
#pragma once

#include "entt/entity/registry.hpp"
#include "raylib.h"

namespace Archetypes {

  entt::entity createTile(
    entt::registry &registry,
    Texture const&,
    Vector2 pos,
    Rectangle quad,
    float alpha,
    int layerIndex = 0
  );

}