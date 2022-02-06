
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"
#include "SelectionBoxModel.hh"

class MultiSelectTool : public ITileTool {
private:
  entt::registry &registry_;
  entt::entity selectionBox_;
  SelectionBoxModel selectModel_;


public:
  MultiSelectTool(entt::registry &);

  void execute() override;
};
