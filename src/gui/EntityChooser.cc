
#include "EntityChooser.hh"

#include "raygui.h"
#include "Components.hh"

EntityChooser::EntityChooser(entt::registry &reg) : entityWindow_(reg), registry_(reg) {}

bool EntityChooser::render() {
  mousepos_ = GetMousePosition();
  mousepressed_ = IsMouseButtonPressed(0);

  if (bool isClicked = GuiWindowBox(windowBoundary_, "Entity Debugger"); isClicked) {
    return false;
  }

  text_.clear();
  auto const view = registry_.view<Components::Name>();
  for (auto const &e : view) {
    Components::Name const &name = view.get<Components::Name>(e);
    text_.push_back(name.name.c_str());
  }

  if (GuiDropdownBoxEx({windowBoundary_.x + 10, windowBoundary_.y + 30, 100, 26},
    text_.data(), static_cast<int>(text_.size()), &selectedIndex_, editable_) ) {
    editable_ = !editable_;
  }

  if (GuiButton({windowBoundary_.x + 10, windowBoundary_.y + (windowBoundary_.height - 36.f), 100, 26}, "Edit entity")) {
    mode_ = EntityMode::edit_mode;
    selected_ = view[selectedIndex_];
  }

  if (mode_ == EntityMode::edit_mode && selected_) {
    if (!entityWindow_.render(selected_.value())) {
      mode_ = EntityMode::no_render;
      selected_.reset();
    }
  }

  if (mousepressed_) {
    header_.x = windowBoundary_.x;
    header_.y = windowBoundary_.y;
    if (CheckCollisionPointRec(mousepos_, header_)) {
      isDragging_ = true;
    }
    auto const &entity = findEntity(registry_, mousepos_);
    if (entity) {
      mode_ = EntityMode::edit_mode;
      selected_ = entity;
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

std::optional<entt::entity> EntityChooser::findEntity(entt::registry &registry, Vector2 &mousePos) {
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