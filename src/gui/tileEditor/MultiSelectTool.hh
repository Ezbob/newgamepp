
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"
#include "SelectionBoxModel.hh"
#include <vector>

class MultiSelectTool : public ITileTool {
private:
  entt::registry &registry_;
  entt::entity selectionBox_;
  int &currentLayer_;
  std::vector<entt::entity> &selections_;
  Camera2D &camera_;

public:
  MultiSelectTool(entt::registry &, int &, std::vector<entt::entity> &, Camera2D &);

  void execute() override;
};
