
#include "DebugMenu.hh"

#include "raygui.h"

DebugMenu::DebugMenu(entt::registry &reg) 
  : entityWindow_(reg), tileWindow_(reg, nfdFileOpener_)
{}

bool DebugMenu::render() {

  if (bool isClicked = GuiWindowBox(windowBoundary_, "Debug menu"); isClicked) {
    return false;
  }

  if (GuiButton({windowBoundary_.x + 10.f, windowBoundary_.y + 35.f, windowBoundary_.width - 20.f, 26}, "Edit entities")) {
    currentWindow_ = &entityWindow_;
  }

  if (GuiButton({windowBoundary_.x + 10.f, windowBoundary_.y + (2.f * 35.f), windowBoundary_.width - 20.f, 26}, "Edit tiles")) {
    currentWindow_ = &tileWindow_;
  }

  if (currentWindow_ && !currentWindow_->render()) {
    currentWindow_ = nullptr;
  }

  return true;
}
