
#pragma once

#include "ITileTool.hh"
#include "entt/entity/registry.hpp"
#include "SelectedCollection.hh"


class RemoveTool : public ITileTool {
private:
  entt::registry &registry_;
  Camera2D &camera_;
  int &currentLayerId_;
  SelectedCollection &selected_;

public:
  RemoveTool(entt::registry &reg, Camera2D &camera, int &currentLayerId, SelectedCollection &);

  void execute() override;
};