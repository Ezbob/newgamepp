
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"


class RemoveTool : public ITileTool {
private:
  entt::registry &registry_;
  Camera2D &camera_;
  int &currentLayerId_;
  std::vector<entt::entity> &selected_;

public:
  RemoveTool(entt::registry &reg, Camera2D &camera, int &currentLayerId, std::vector<entt::entity> &);

  void execute() override;
};