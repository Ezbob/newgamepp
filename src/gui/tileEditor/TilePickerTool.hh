
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"
#include <vector>

class TilePickerTool : public ITileTool {
private:
  entt::registry &registry_;
  std::vector<entt::entity> &selectedTiles_;
  Camera2D &camera_;
  int &currentLayerId_;
public:
  TilePickerTool(entt::registry &, Camera2D &, int &, std::vector<entt::entity> &);

  void execute() override;
};