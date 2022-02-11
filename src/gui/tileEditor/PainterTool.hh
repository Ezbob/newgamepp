
#pragma once

#include "ITileTool.hh"
#include "SelectedCollection.hh"
#include "TileSetSelector.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"

class PainterTool : public ITileTool {
private:
  entt::registry &registry_;
  TileSetSelector &selector_;
  Camera2D &camera_;
  SelectedCollection &selectedTiles_;
  int &currentLayer_;

  int roundDownTo(int N, int n) const;

  Vector2 midPoint(Rectangle r) const;

public:
  PainterTool(entt::registry &, TileSetSelector &, Camera2D &,
              SelectedCollection &, int &);

  void renderToolAttributes(Rectangle const&) override;

  void execute() override;
};