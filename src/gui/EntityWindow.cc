
#include "EntityWindow.hh"
#include <charconv>

#include "raygui.h"
#include <array>
#include <cstdlib>
#include "fmt/core.h"

namespace {

  void renderTextField(Rectangle bounds, bool mousepressed, Vector2 &mousepos, int &data, bool &editable) {

    if (mousepressed) {
      if (CheckCollisionPointRec(mousepos, bounds)) {
        editable = true;
      } else {
        editable = false;
      }
    }
    std::array<char, 128> buffer = {};
    buffer.fill('\0');

    std::to_chars(buffer.data(), buffer.data() + buffer.size(), data);

    if (GuiTextBox(bounds, buffer.data(), buffer.size(), editable)) {
      editable = !editable;
    }

    std::from_chars(buffer.data(), buffer.data() + buffer.size(), data, 10);
  }


  void renderTextField(Rectangle bounds, bool mousepressed, Vector2 &mousepos, float &data, bool &editable) {

    if (mousepressed) {
      if (CheckCollisionPointRec(mousepos, bounds)) {
        editable = true;
      } else {
        editable = false;
      }
    }

    std::string buffer = fmt::format("{}", data);

    GuiTextBox(bounds, buffer.data(), buffer.size(), false);

    data = std::stof(buffer);
  }

  void renderTextField(Rectangle bounds, bool mousepressed, Vector2 &mousepos, std::string &data, bool &editable) {
    std::array<char, 128> buffer = {'\0'};

    if (mousepressed) {
      if (CheckCollisionPointRec(mousepos, bounds)) {
        editable = true;
      } else {
        editable = false;
      }
    }

    if (!editable) {
      GuiTextBox(bounds, data.data(), static_cast<int>(data.size()), editable);
    } else {
      data.copy(buffer.data(), buffer.size());

      GuiTextBox(bounds, buffer.data(), static_cast<int>(buffer.size()), editable);

      data = std::string(buffer.data());
    }
  }

  std::optional<entt::entity> findEntity(entt::registry &registry, Vector2 &mousePos) {
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
};

EntityWindow::EntityWindow(entt::registry &r) : registry_(r) {}

void EntityWindow::drawEntity() {

  if (!selected_) {
    GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + (35.f * 2.f), 120, 25}, "No entity selected...");
    return;
  }

  auto entity = selected_.value();

  int i = 1;

  float margin = 0.f;

  if (registry_.any_of<Components::Name>(entity)) {
    auto &name = registry_.get<Components::Name>(entity);

    i++;
    GuiLabel({windowBoundary_.x + 10, windowBoundary_.y + (35.f * static_cast<float>(i)), 120, 25}, "Name: ");
    renderTextField({windowBoundary_.x + 70, windowBoundary_.y + (35.f * static_cast<float>(i)), 120, 25}, mousepressed_, mousepos_, name.name, nameFieldEditable_);

    margin += 12.f;
  }

  if (registry_.any_of<Components::Position>(entity)) {
    auto &pos = registry_.get<Components::Position>(entity);
    i++;
    GuiGroupBox({windowBoundary_.x + 5, windowBoundary_.y + 35.f * (static_cast<float>(i)) + margin, 380, 75.f},
                "Position");

    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, "X: ");
    renderTextField({windowBoundary_.x + 70, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, mousepressed_, mousepos_, pos.x, positionXFieldEditable_);

    if (GuiButton({ windowBoundary_.x + 380 + 15, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 60, 25 }, "reset")) {
      pos.x = 0.f;
    }

    i++;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, "Y: ");
    renderTextField({windowBoundary_.x + 70, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, mousepressed_, mousepos_, pos.y, positionYFieldEditable_);

    if (GuiButton({ windowBoundary_.x + 380 + 15, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 60, 25 }, "reset")) {
      pos.y = 0.f;
    }
    margin += 12.f;
  }

  if (registry_.any_of<Components::Velocity>(entity)) {
    auto &vel = registry_.get<Components::Velocity>(entity);
    ++i;
    GuiGroupBox({windowBoundary_.x + 5, windowBoundary_.y + 35.f * (static_cast<float>(i)) + margin, 380, 75.f},"Velocity");

    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + 36.f * static_cast<float>(i) + margin, 120, 25}, "X: ");
    renderTextField({windowBoundary_.x + 70, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, mousepressed_, mousepos_, vel.dx, velocityXFieldEditable_);

    vel.dx = GuiSlider({windowBoundary_.x + 230, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, "Min", "Max", vel.dx, -300, 300);

    if (GuiButton({ windowBoundary_.x + 380 + 15, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 60, 25 }, "reset")) {
      vel.dx = 0.f;
    }

    ++i;
    GuiLabel({windowBoundary_.x + 15, windowBoundary_.y + 36.f * static_cast<float>(i) + margin, 120, 25}, "Y: ");
    renderTextField({windowBoundary_.x + 70, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, mousepressed_, mousepos_, vel.dy, velocityYFieldEditable_);

    vel.dy = GuiSlider({windowBoundary_.x + 230, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 120, 25}, "Min", "Max", vel.dy, -300, 300);

    if (GuiButton({ windowBoundary_.x + 380 + 15, windowBoundary_.y + (36.f * static_cast<float>(i)) + margin, 60, 25 }, "reset")) {
      vel.dy = 0.f;
    }
  }

  if (registry_.any_of<Components::Position, Components::Dimensions>(entity)) {
    auto &pos = registry_.get<Components::Position>(entity);
    auto &dim = registry_.get<Components::Dimensions>(entity);

    DrawRectangleLinesEx({(float) pos.x - 5.f, (float) pos.y - 5.f, (float) dim.w + 10.f, (float) dim.h + 10.f}, 1, GREEN);
  }
}

bool EntityWindow::render() {
  mousepos_ = GetMousePosition();
  mousepressed_ = IsMouseButtonPressed(0);

  if (GuiWindowBox(windowBoundary_, "Entity Debugger")) {
    return false;
  }

  drawEntity();

  text_.clear();
  auto const view = registry_.view<Components::Name>();

  text_.push_back("<no entity>");

  int found_index = 1;
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

  GuiLabel({windowBoundary_.x + 10.f, windowBoundary_.y + 32.f, 150.f, 26.f}, "Choose entity:");
  if (GuiDropdownBoxEx({windowBoundary_.x + windowBoundary_.width - 10.f - 150.f, windowBoundary_.y + 32.f, 150.f, 26.f},
                       text_.data(), static_cast<int>(text_.size()), &selectedIndex_, editable_) ) {
    editable_ = !editable_;
  }

  if (selectedIndex_ > 0) {
    selected_ = view[selectedIndex_ - 1];
  } else if (selectedIndex_ == 0) {
    selected_.reset();
  }

  if (mousepressed_) {
    auto const &entity = findEntity(registry_, mousepos_);
    if (entity) {
      selected_ = entity;
    }
  }

  return true;
}
