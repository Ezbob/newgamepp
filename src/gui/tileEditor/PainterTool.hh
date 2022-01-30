
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
  TileModel &tileModel_;
  Camera2D &camera_;
  entt::entity &selectedTile_;

  int roundDownTo(int N, int n);

  Vector2 midPoint(Rectangle r);
public:
  PainterTool(entt::registry &, TileSelector &, TileModel &, Camera2D &, entt::entity &);

  void execute() override;
};