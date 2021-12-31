
#pragma once

#include "EntityWindow.hh"
#include "IRenderable.hh"
#include "TileWindow.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include <optional>
#include <vector>
#include "NFDFileOpener.hh"

class DebugMenu : public IRenderable {

public:
  explicit DebugMenu(entt::registry &);
  ~DebugMenu() = default;

  bool render() override;

private:
  Rectangle windowBoundary_{
          0.f,
          Constants::screenHeight,
          200.f,
          200.f};

  EntityWindow entityWindow_;
  TileWindow tileWindow_;

  IRenderable *currentWindow_ = nullptr;
  NFDFileOpener nfdFileOpener_;
};