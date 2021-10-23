
#include "EntityWindow.hh"

#include "raygui.h"

EntityWindow::EntityWindow(entt::registry &r) : registry_(r) {}

void EntityWindow::drawEntity() {
  if (selected_) {
    auto entity = selected_.value();

    int i = 0;
    if (registry_.any_of<Position>(entity)) {
      auto &pos = registry_.get<Position>(entity);

      i++;
      GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + 35 * i, 120, 25}, "Position X: ");
      positionXField_.render(mousepressed_, mousepos_, pos.x, i);

      i++;
      GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + 35 * i, 120, 25}, "Position Y: ");
      positionYField_.render(mousepressed_, mousepos_, pos.y, i);
    }

    if (registry_.any_of<Velocity>(entity)) {
      auto &vel = registry_.get<Velocity>(entity);

      ++i;
      GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + 35 * i, 120, 25}, "Velocity X: ");
      sliderfields_[0].render(mousepressed_, mousepos_, vel.dx, i);

      ++i;
      GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + 35 * i, 120, 25}, "Velocity Y: ");
      sliderfields_[1].render(mousepressed_, mousepos_, vel.dy, i);
    }
  }
}

void EntityWindow::doGui() {
  mousepos_ = GetMousePosition();
  mousepressed_ = IsMouseButtonPressed(0);

  if (mousepressed_) {
    auto view = registry_.view<Position, Dimensions>();

    for (auto ent : view) {
      auto const &pos = view.get<Position>(ent);
      auto const &dim = view.get<Dimensions>(ent);
      Rectangle entityBox {
        (float)(pos.x),
        (float)(pos.y),
        (float)(dim.w),
        (float)(dim.h)
      };
      if (CheckCollisionPointRec(mousepos_, entityBox)) {
        selected_ = ent;
        break;
      }
    }
  }

  if (drawWindow_ && selected_.has_value()) {
    if (GuiWindowBox(windowBoundary_, "Entity Selected")) {
      selected_ = std::nullopt;
    } else {
      drawEntity();
    }
  }

  if (IsKeyPressed('R') && selected_.has_value()) {
    drawWindow_ = !drawWindow_;
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
}