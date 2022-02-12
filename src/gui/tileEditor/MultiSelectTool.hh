
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

  bool cacheReadIn_ = false;
  struct Cache {
    float alpha = 1.f;
    bool vFlip = false;
    bool hFlip = false;

    void reset() {
      alpha = 1.f;
      vFlip = false;
      hFlip = false;
    }
  } cache_;

public:
  MultiSelectTool(entt::registry &, int &, SelectedCollection &, Camera2D &);

  void execute() override;

  void onSelected() override;

  void renderToolAttributes(Rectangle const&) override;
};
