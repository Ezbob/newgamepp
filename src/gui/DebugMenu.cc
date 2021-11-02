
#include "DebugMenu.hh"

#include "raygui.h"

DebugMenu::DebugMenu(entt::registry &reg) : entityWindow_(reg) {}

bool DebugMenu::render() {
  mousepos_ = GetMousePosition();
  mousepressed_ = IsMouseButtonPressed(0);

  if (bool isClicked = GuiWindowBox(windowBoundary_, "Menu Debugger"); isClicked) {
    return false;
  }

  if (GuiButton({windowBoundary_.x + 10.f, windowBoundary_.y + 35.f, windowBoundary_.width - 20.f, 26}, "Edit entities") && mode_ == EntityMode::no_render) {
    mode_ = EntityMode::entity_edit_mode;
  }

  if (mode_ == EntityMode::entity_edit_mode) {
    if (!entityWindow_.render()) {
      mode_ = EntityMode::no_render;
    }
  }

  if (mousepressed_) {
    header_.x = windowBoundary_.x;
    header_.y = windowBoundary_.y;
    if (CheckCollisionPointRec(mousepos_, header_)) {
      isDragging_ = true;
    }
  }

  if (IsMouseButtonUp(0)) {
    isDragging_ = false;
  }

  if (IsMouseButtonDown(0)) {
    header_.x = windowBoundary_.x;
    header_.y = windowBoundary_.y;
    if (isDragging_) {
      windowBoundary_.x += (mousepos_.x - windowBoundary_.x) - (header_.width / 2);
      windowBoundary_.y += (mousepos_.y - windowBoundary_.y) - (header_.height / 2);
    }
  }

  return true;
}
