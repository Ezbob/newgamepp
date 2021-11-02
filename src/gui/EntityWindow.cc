
#include "EntityWindow.hh"

#include "raygui.h"

EntityWindow::EntityWindow(entt::registry &r) : registry_(r) {}

void EntityWindow::drawEntity() {

  if (!selected_) {
    return;
  }

  auto entity = selected_.value();

  int i = 1;

  if (registry_.any_of<Components::Name>(entity)) {
    auto &name = registry_.get<Components::Name>(entity);

    i++;
    GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + (35.f * static_cast<float>(i)), 120, 25}, "Name: ");
    nameField_.render(mousepressed_, mousepos_, name.name, i);
  }

  if (registry_.any_of<Components::Position>(entity)) {
    auto &pos = registry_.get<Components::Position>(entity);

    GuiGroupBox({windowBoundary_.x + 5, windowBoundary_.y + 33.f * (1.f + static_cast<float>(i)), 380, 70.f},
                "Position");
    i++;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + (35.f * static_cast<float>(i)), 120, 25}, "X: ");
    positionXField_.render(mousepressed_, mousepos_, pos.x, i);

    i++;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + (35.f * static_cast<float>(i)), 120, 25}, "Y: ");
    positionYField_.render(mousepressed_, mousepos_, pos.y, i);
  }

  if (registry_.any_of<Components::Velocity>(entity)) {
    auto &vel = registry_.get<Components::Velocity>(entity);

    float spacing = (i > 0) ? 12.f : 0.f;

    GuiGroupBox({windowBoundary_.x + 5, windowBoundary_.y + spacing + 33.f * (1.f + static_cast<float>(i)), 380, 74.f},
                "Velocity");
    ++i;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + spacing + 35.f * static_cast<float>(i), 120, 25}, "X: ");
    velocityXField_.render(mousepressed_, mousepos_, vel.dx, i);
    sliderVelocityXField_.render(mousepressed_, mousepos_, vel.dx, i);

    ++i;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + spacing + 35.f * static_cast<float>(i), 120, 25}, "Y: ");
    velocityYField_.render(mousepressed_, mousepos_, vel.dy, i);
    sliderVelocityYField_.render(mousepressed_, mousepos_, vel.dy, i);
  }

  if (registry_.any_of<Components::Position, Components::Dimensions>(entity)) {
    auto &pos = registry_.get<Components::Position>(entity);
    auto &dim = registry_.get<Components::Dimensions>(entity);

    DrawRectangleLinesEx({(float) pos.x - 5.f, (float) pos.y - 5.f, (float) dim.w + 10.f, (float) dim.h + 10.f}, 1,
                         GREEN);
  }
}

bool EntityWindow::render() {
  mousepos_ = GetMousePosition();
  mousepressed_ = IsMouseButtonPressed(0);

  if (GuiWindowBox(windowBoundary_, "Entity Selected")) {
    return false;
  }

  text_.clear();
  auto const view = registry_.view<Components::Name>();

  int found_index = 0;
  for (auto const &e : view) {
    Components::Name const &name = view.get<Components::Name>(e);
    text_.push_back(name.name.c_str());
    if (selected_ && registry_.any_of<Components::Name>(selected_.value())) {
      auto const &otherName = registry_.get<Components::Name>(selected_.value());
      if (name.name == otherName.name) {
         selectedIndex_ = found_index;
      }
    }
    found_index++;
  }

  GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + 30, 100, 26}, "Choose entity:");
  if (GuiDropdownBoxEx({windowBoundary_.x + windowBoundary_.width - 110.f, windowBoundary_.y + 30, 100, 26},
                       text_.data(), static_cast<int>(text_.size()), &selectedIndex_, editable_) ) {
    editable_ = !editable_;
  }

  selected_ = view[selectedIndex_];

  if (mousepressed_) {
    header_.x = windowBoundary_.x;
    header_.y = windowBoundary_.y;
    if (CheckCollisionPointRec(mousepos_, header_)) {
      isDragging_ = true;
    }
    auto const &entity = findEntity(registry_, mousepos_);
    if (entity) {
      selected_ = entity;
    }
  }

  drawEntity();

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

std::optional<entt::entity> EntityWindow::findEntity(entt::registry &registry, Vector2 &mousePos) const {
  auto view = registry.view<Components::Position, Components::Dimensions>();

  for (auto ent : view) {
    auto const &pos = view.get<Components::Position>(ent);
    auto const &dim = view.get<Components::Dimensions>(ent);

    if (CheckCollisionPointRec(mousePos, {pos.x, pos.y, dim.w, dim.h})) {
      return ent;
    }
  }
  return std::nullopt;
}