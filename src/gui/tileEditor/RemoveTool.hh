
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"


class RemoveTool : public ITileTool {
private:
  entt::registry &registry_;
  Camera2D &camera_;
  int &currentLayerId_;

public:
  RemoveTool(entt::registry &reg, Camera2D &camera, int &currentLayerId);

  void execute() override;
};