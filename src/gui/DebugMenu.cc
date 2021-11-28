
#include "DebugMenu.hh"

#include "raygui.h"

DebugMenu::DebugMenu(entt::registry &reg) : entityWindow_(reg) {}

bool DebugMenu::render() {

  if (bool isClicked = GuiWindowBox(windowBoundary_, "Debug menu"); isClicked) {
    return false;
  }

  if (GuiButton({windowBoundary_.x + 10.f, windowBoundary_.y + 35.f, windowBoundary_.width - 20.f, 26.f}, "Edit entities")) {
    window_choice_ = DebugWindowChoice::entity_window;
  }

  if (GuiButton({windowBoundary_.x + 10.f, windowBoundary_.y + 35.f + 36.f, windowBoundary_.width - 20.f, 26.f}, "Edit tiles")) {
    window_choice_ = DebugWindowChoice::tile_window;
  }

  if (window_choice_ == DebugWindowChoice::entity_window && !entityWindow_.render()) {
    window_choice_ = DebugWindowChoice::no_window;
  }

  if (window_choice_ == DebugWindowChoice::tile_window && !tileWindow_.render()) {
    window_choice_ = DebugWindowChoice::no_window;
  }

  return true;
}
