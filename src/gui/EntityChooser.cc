
#include "EntityChooser.hh"

#include "raygui.h"
#include "Components.hh"

EntityChooser::EntityChooser(entt::registry &reg) : entityWindow_(reg), registry_(reg) {}

bool EntityChooser::render() {
  mousepos_ = GetMousePosition();
  mousepressed_ = IsMouseButtonPressed(0);

  if (drawWindow_) {
    if ( GuiWindowBox(windowBoundary_, "Entity selector") ) {
      drawWindow_ = false;
    }

    if ( GuiDropdownBox({windowBoundary_.x + 10, windowBoundary_.y + 26, 100, 26}, "ONE;TWO;THREE", &a, editable_) ) {
      editable_ = !editable_;
    }
  }

  if (mousepressed_) {
    Vector2 mousepos = GetMousePosition();
    auto entityFound = findEntity(registry_, mousepos);
    if (entityFound) {
      selected_ = entityFound;
    }
  }

  if (selected_) {
    bool isNotClosed = entityWindow_.render(selected_.value());
    if (!isNotClosed) {
        selected_ = std::nullopt;
    }
  }

  if (IsKeyPressed('R')) {
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

  return true;
}

std::optional<entt::entity> EntityChooser::findEntity(entt::registry &registry, Vector2 &mousePos) {
    auto view = registry.view<Position, Dimensions>();

    for (auto ent : view) {
        auto const &pos = view.get<Position>(ent);
        auto const &dim = view.get<Dimensions>(ent);

        if (CheckCollisionPointRec(mousePos, {pos.x, pos.y, dim.w, dim.h})) {
            return ent;
        }
    }
    return std::nullopt;
}