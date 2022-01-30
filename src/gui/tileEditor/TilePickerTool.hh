
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"
#include "TileModel.hh"

class TilePickerTool : public ITileTool {
private:
  entt::registry &registry_;
  entt::entity &selectedTile_;
  Camera2D &camera_;
  int &currentLayerId_;
  TileModel &tileModel_;
public:
  TilePickerTool(entt::registry &, Camera2D &, int &, entt::entity &, TileModel &);

  void execute() override;
};