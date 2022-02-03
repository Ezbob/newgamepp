
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"

class MultiSelectTool : public ITileTool {
private:
  entt::registry &registry_;
public:
  MultiSelectTool(entt::registry &);

  void execute() override;
};
