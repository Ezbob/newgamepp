
#include "EntityWindow.hh"

#include "raygui.h"

EntityWindow::EntityWindow(entt::registry &r) : registry_(r) {}

void EntityWindow::drawEntity(entt::entity &entity) {

  int i = 0;

  if (registry_.any_of<Name>(entity)) {
    auto &name = registry_.get<Name>(entity);

    i++;
    GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + 35 * i, 120, 25}, "Name: ");
    nameField_.render(mousepressed_, mousepos_, name.name, i);
  }

  if (registry_.any_of<Position>(entity)) {
    auto &pos = registry_.get<Position>(entity);

    GuiGroupBox({windowBoundary_.x + 5, windowBoundary_.y + 33.f * (1 + i), 380, 70.f}, "Position");
    i++;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + 35.f * i, 120, 25}, "X: ");
    positionXField_.render(mousepressed_, mousepos_, pos.x, i);

    i++;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + 35.f * i, 120, 25}, "Y: ");
    positionYField_.render(mousepressed_, mousepos_, pos.y, i);
  }

  if (registry_.any_of<Velocity>(entity)) {
    auto &vel = registry_.get<Velocity>(entity);

    float spacing = (i > 0) ? 12.f : 0.f;

    GuiGroupBox({windowBoundary_.x + 5, windowBoundary_.y + spacing + 33.f * (1 + i), 380, 74.f}, "Velocity");
    ++i;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + spacing + 35.f * i, 120, 25}, "X: ");
    velocityXField_.render(mousepressed_,  mousepos_, vel.dx, i);
    sliderVelocityXField_.render(mousepressed_, mousepos_, vel.dx, i);

    ++i;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + spacing + 35.f * i, 120, 25}, "Y: ");
    velocityYField_.render(mousepressed_,  mousepos_, vel.dy, i);
    sliderVelocityYField_.render(mousepressed_, mousepos_, vel.dy, i);
  }

  if (registry_.any_of<Position, Dimensions>(entity)) {
    auto &pos = registry_.get<Position>(entity);
    auto &dim = registry_.get<Dimensions>(entity);

    DrawRectangleLinesEx({(float)pos.x - 5.f, (float)pos.y - 5.f, (float)dim.w + 10.f, (float)dim.h + 10.f}, 1, GREEN);
  }

}

bool EntityWindow::render(entt::entity &entity) {
  mousepos_ = GetMousePosition();
  mousepressed_ = IsMouseButtonPressed(0);

  if (drawWindow_) {
    if (GuiWindowBox(windowBoundary_, "Entity Selected")) {
      return false;
    }

    drawEntity(entity);
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
