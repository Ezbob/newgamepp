
#pragma once

#include "ITileTool.hh"
#include "TileSelector.hh"
#include "raylib.h"
#include "TileModel.hh"
#include "entt/entity/registry.hpp"

class PainterTool : public ITileTool {
private:
  entt::registry &registry_;
  TileSelector &selector_;
  Camera2D &camera_;
  std::vector<entt::entity> &selectedTiles_;
  int &currentLayer_;

  int roundDownTo(int N, int n) const;

  Vector2 midPoint(Rectangle r) const;
public:
  PainterTool(entt::registry &, TileSelector &, Camera2D &,
    std::vector<entt::entity> &, int &);

  void execute() override;
};