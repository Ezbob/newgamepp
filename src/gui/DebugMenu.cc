
#include "DebugMenu.hh"

#include "raygui.h"

DebugMenu::DebugMenu(entt::registry &reg) : entityWindow_(reg) {}

bool DebugMenu::render() {

  if (bool isClicked = GuiWindowBox(windowBoundary_, "Debug menu"); isClicked) {
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

  return true;
}
