
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"
#include "SelectedCollection.hh"

class MultiSelectTool : public ITileTool {
private:
  entt::registry &registry_;
  entt::entity selectionBox_;
  int &currentLayer_;
  SelectedCollection &selections_;
  Camera2D &camera_;

public:
  MultiSelectTool(entt::registry &, int &, SelectedCollection &, Camera2D &);

  void execute() override;
};
