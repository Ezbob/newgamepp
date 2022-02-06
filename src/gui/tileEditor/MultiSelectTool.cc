
#include "MultiSelectTool.hh"
#include "TilePicker.hh"
#include "raylib.h"
#include "raygui.h"
#include "Constants.hh"

MultiSelectTool::MultiSelectTool(entt::registry &reg) : registry_(reg) {
  selectionBox_ = selectModel_.create(reg);
}

void MultiSelectTool::execute() {
  auto mousePosition = GetMousePosition();

  if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {

    if (IsMouseButtonPressed(0)) {
      selectModel_.boundary.x = mousePosition.x;
      selectModel_.boundary.y = mousePosition.y;
      selectModel_.show = true;
    }

    if (selectModel_.show && IsMouseButtonDown(0)) {
      selectModel_.boundary.width = (mousePosition.x - selectModel_.boundary.x);
      selectModel_.boundary.height = (mousePosition.y - selectModel_.boundary.y);
    }

    if (selectModel_.show && IsMouseButtonUp(0)) {
      selectModel_.boundary.width = (mousePosition.x - selectModel_.boundary.x);
      selectModel_.boundary.height = (mousePosition.y - selectModel_.boundary.y);
      selectModel_.show = false;
    }

  }

  selectModel_.update(registry_, selectionBox_);
}
