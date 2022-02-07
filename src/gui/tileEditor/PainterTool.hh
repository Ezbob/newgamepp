
#pragma once

#include "ITileTool.hh"
#include "TileSelector.hh"
#include "raylib.h"
#include "entt/entity/registry.hpp"
#include "SelectedCollection.hh"

class PainterTool : public ITileTool {
private:
  entt::registry &registry_;
  TileSelector &selector_;
  Camera2D &camera_;
  SelectedCollection &selectedTiles_;
  int &currentLayer_;

  int roundDownTo(int N, int n) const;

  Vector2 midPoint(Rectangle r) const;
public:
  PainterTool(entt::registry &, TileSelector &, Camera2D &,
    SelectedCollection &, int &);

  void execute() override;
};