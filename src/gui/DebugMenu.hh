
#pragma once

#include "EntityWindow.hh"
#include "IRenderable.hh"
#include "tileEditor/TileWindow.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include <optional>
#include <vector>
#include "NFDFileOpener.hh"

class DebugMenu : public IRenderable {

public:
  explicit DebugMenu(entt::registry &, Camera2D &);
  ~DebugMenu() = default;

  bool render() override;

private:
  Rectangle windowBoundary_{
          0.f,
          Constants::screenHeight,
          200.f,
          180.f};

  EntityWindow entityWindow_;
  TileWindow tileWindow_;

  IRenderable *currentWindow_ = nullptr;
  NFDFileOpener nfdFileOpener_;
  Camera2D &camera_;
};