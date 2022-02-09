
#pragma once

#include <array>
#include <optional>

#include "Components.hh"
#include "Constants.hh"
#include "IRenderable.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"

class EntityWindow : public IRenderable {
private:
  Rectangle windowBoundary_{
          Constants::screenWidth,
          0.f,
          600.f,
          Constants::screenHeight + 180.f};

  Vector2 mousepos_ = {0.f, 0.f};

  int selectedIndex_ = 0;
  std::vector<const char *> text_;
  bool editable_ = false;
  bool mousepressed_ = false;

  entt::registry &registry_;
  std::optional<entt::entity> selected_;

  Camera2D &camera_;

  bool nameFieldEditable_ = false;

  bool velocityXFieldEditable_ = false;
  bool velocityYFieldEditable_ = false;

  bool positionXFieldEditable_ = false;
  bool positionYFieldEditable_ = false;

  bool leftScreenClampEditable_ = false;
  bool rightScreenClampEditable_ = false;
  bool bottomScreenClampEditable_ = false;
  bool topScreenClampEditable_ = false;


  void drawEntity();

public:
  explicit EntityWindow(entt::registry &, Camera2D &);

  bool render();
};