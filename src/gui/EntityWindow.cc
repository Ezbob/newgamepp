
#include "EntityWindow.hh"
#include <charconv>

#include "raygui.h"
#include <array>
#include <cstdlib>
#include "fmt/core.h"

namespace {

  void renderTextField(Rectangle bounds, bool mousepressed, Vector2 &mousepos, float &data, bool &editable) {

    if (mousepressed) {
      if (CheckCollisionPointRec(mousepos, bounds)) {
        editable = true;
      } else {
        editable = false;
      }
    }

    if (GuiValueBoxEx(bounds, nullptr, &data, -1e15f, 1e15f, false)) {
      editable = !editable;
    }
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

  std::optional<entt::entity> findEntityByClickedRectangle(entt::registry &registry, Vector2 &mousePos) {
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

  Rectangle rowLocation(Rectangle const& parent, float xPadding, float yPadding, float width, float height, int row = 1, float margin = 0.f) {
    return { parent.x + xPadding, parent.y + (yPadding * static_cast<float>(row)) + margin, width, height };
  }
}

EntityWindow::EntityWindow(entt::registry &r) : registry_(r) {}

void EntityWindow::drawEntity() {

  if (!selected_) {
    GuiLabel(rowLocation(windowBoundary_, 10, 35.f * 2.f, 120, 25), "No entity selected...");
    return;
  }

  auto entity = selected_.value();

  int i = 1;

  // for the entity to be found, we need a name
  assert(registry_.any_of<Components::Name>(entity));

  auto &name = registry_.get<Components::Name>(entity);

  i++;
  GuiLabel(rowLocation(windowBoundary_, 10.f, 33.f, 120.f, 25.f, i), "Name: ");
  renderTextField(rowLocation(windowBoundary_, 55.f, 33.f, 120.f, 25.f, i), mousepressed_, mousepos_, name.name, nameFieldEditable_);

  auto fieldBounds = [this](int row, float margin = 0.f) {
    return rowLocation(windowBoundary_, 70.f, 40.f, 120.f, 25.f, row, margin);
  };

  auto labelBounds = [this](int row, float margin = 0.f) {
    return rowLocation(windowBoundary_, 15.f, 40.f, 120.f, 25.f, row, margin);
  };

  auto buttonBounds = [this](int row, float margin = 0.f) {
    return rowLocation(windowBoundary_, windowBoundary_.width - 10.f - 60.f, 40.f, 60.f, 25.f, row, margin);
  };

  auto groupBounds = [this](int row, int nfields, float margin = 0.f) {
    return rowLocation(windowBoundary_, 5.f, 38.f, windowBoundary_.width - 10.f, 40.f * static_cast<float>(nfields), row, margin);
  };

  auto sliderBounds = [this](int row, float margin = 0.f) {
    return rowLocation(windowBoundary_, 330.f, 40.f, 120.f, 25.f, row, margin);
  };

  auto sliderLabelBounds = [this](int row, float margin = 0.f) {
    return rowLocation(windowBoundary_, 230.f, 40.f, 120.f, 25.f, row, margin);
  };

  if (registry_.any_of<Components::Position>(entity)) {
    auto &pos = registry_.get<Components::Position>(entity);
    i++;

    GuiLabel(labelBounds(i), "X: ");
    renderTextField(fieldBounds(i), mousepressed_, mousepos_, pos.x, positionXFieldEditable_);

    if (GuiButton(buttonBounds(i), "reset")) {
      pos.x = 0.f;
    }

    i++;
    GuiLabel(labelBounds(i), "Y: ");
    renderTextField(fieldBounds(i), mousepressed_, mousepos_, pos.y, positionYFieldEditable_);

    if (GuiButton(buttonBounds(i), "reset")) {
      pos.y = 0.f;
    }

    GuiGroupBox(groupBounds(i - 1, 2), "Position");
  }

  float marginBox = 12.f;
  float marginFields = 10.f;

  if (registry_.any_of<Components::Velocity>(entity)) {
    auto &vel = registry_.get<Components::Velocity>(entity);
    ++i;
    GuiGroupBox(groupBounds(i, 2, marginBox),"Velocity");
    GuiLabel(labelBounds(i, marginFields), "X: ");

    renderTextField(fieldBounds(i, marginFields), mousepressed_, mousepos_, vel.dx, velocityXFieldEditable_);

    vel.dx = GuiSlider(sliderBounds(i, marginFields), "Min", "Max", vel.dx, -300.f, 300.f);

    if (GuiButton(buttonBounds(i, marginFields), "reset")) {
      vel.dx = 0.f;
    }

    ++i;
    GuiLabel(labelBounds(i, marginFields), "Y: ");
    renderTextField(fieldBounds(i, marginFields), mousepressed_, mousepos_, vel.dy, velocityYFieldEditable_);

    vel.dy = GuiSlider(sliderBounds(i, marginFields), "Min", "Max", vel.dy, -300.f, 300.f);

    if (GuiButton(buttonBounds(i, marginFields), "reset")) {
      vel.dy = 0.f;
    }

    marginFields *= 2;
    marginBox *= 2;
  }

  if (registry_.any_of<Components::ScreenClamp>(entity)) {
    auto &clamp = registry_.get<Components::ScreenClamp>(entity);
    ++i;
    GuiGroupBox(groupBounds(i, 4, marginBox),"Screen Clamp");

    GuiLabel(labelBounds(i, marginFields), "left: ");
    renderTextField(fieldBounds(i, marginFields), mousepressed_, mousepos_, clamp.left, leftScreenClampEditable_);

    clamp.left = GuiSlider(sliderBounds(i, marginFields), "Min", "Max", clamp.left, -300.f, 300.f);

    if (GuiButton(buttonBounds(i, marginFields), "reset")) {
      clamp.left = 0.f;
    }

    ++i;
    GuiLabel(labelBounds(i, marginFields), "bottom: ");
    renderTextField(fieldBounds(i, marginFields), mousepressed_, mousepos_, clamp.bottom, bottomScreenClampEditable_);

    clamp.bottom = GuiSlider(sliderBounds(i, marginFields), "Min", "Max", clamp.bottom, -300.f, 300.f);

    if (GuiButton(buttonBounds(i, marginFields), "reset")) {
      clamp.bottom = 0.f;
    }

    ++i;
    GuiLabel(labelBounds(i, marginFields), "right: ");
    renderTextField(fieldBounds(i, marginFields), mousepressed_, mousepos_, clamp.right, rightScreenClampEditable_);

    clamp.right = GuiSlider(sliderBounds(i, marginFields), "Min", "Max", clamp.right, -300.f, 300.f);

    if (GuiButton(buttonBounds(i, marginFields), "reset")) {
      clamp.right = 0.f;
    }

    ++i;
    GuiLabel(labelBounds(i, marginFields), "top: ");
    renderTextField(fieldBounds(i, marginFields), mousepressed_, mousepos_, clamp.top, topScreenClampEditable_);

    clamp.top = GuiSlider(sliderBounds(i, marginFields), "Min", "Max", clamp.top, -300.f, 300.f);

    if (GuiButton(buttonBounds(i, marginFields), "reset")) {
      clamp.top = 0.f;
    }

    DrawRectangleLinesEx({clamp.left, clamp.top,Constants::screenWidth - (clamp.right + clamp.left),Constants::screenHeight - (clamp.top + clamp.bottom)},1, RED);
  }

  /* drawing selection box */
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

  GuiLabel({windowBoundary_.x + 10.f, windowBoundary_.y + 30.f, 150.f, 26.f}, "Choose entity:");
  if (GuiDropdownBoxEx({windowBoundary_.x + windowBoundary_.width - 10.f - 150.f, windowBoundary_.y + 30.f, 150.f, 26.f},
                       text_.data(), static_cast<int>(text_.size()), &selectedIndex_, editable_) ) {
    editable_ = !editable_;
  }

  if (selectedIndex_ > 0) {
    selected_ = view[selectedIndex_ - 1];
  } else if (selectedIndex_ == 0) {
    selected_.reset();
  }

  if (mousepressed_) {
    auto const &entity = findEntityByClickedRectangle(registry_, mousepos_);
    if (entity) {
      selected_ = entity;
    }
  }

  return true;
}
