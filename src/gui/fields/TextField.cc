
#include "TextField.hh"
#include <charconv>
#include "raygui.h"


TextField::TextField(Rectangle &parent, Rectangle bounds)
  : parent_(parent)
  , bounds_(bounds)
  , buffer_(16, '\0')
  {}

void TextField::render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex) {
    Rectangle bounds = {
      parent_.x + bounds_.x,
      parent_.y + bounds_.y * static_cast<float>(stackIndex),
      bounds_.width,
      bounds_.height
    };

    if (mousepressed) {
      if (CheckCollisionPointRec(mousepos, bounds)) {
        editable_ = true;
      } else {
        editable_ = false;
      }
    }
    if (!editable_) {
      std::to_chars(buffer_.data(), buffer_.data() + buffer_.size(), data);
    }
    GuiTextBox(bounds, buffer_.data(), static_cast<int>(buffer_.size()), editable_);

    if ( editable_ ) {
        float value = 0.0f;
        auto [ptr, ec] = std::from_chars(buffer_.data(), buffer_.data() + buffer_.size(), value);
        if (ec == std::errc())
        {
            data = value;
        }
    }
}


void TextField::render(bool mousepressed, Vector2 &mousepos, std::string &data, int stackIndex) {
    Rectangle bounds = {
      parent_.x + bounds_.x,
      parent_.y + bounds_.y * static_cast<float>(stackIndex),
      bounds_.width,
      bounds_.height
    };

    if (mousepressed) {
      if (CheckCollisionPointRec(mousepos, bounds)) {
        editable_ = true;
      } else {
        editable_ = false;
      }
    }

    if (!editable_) {
      GuiTextBox(bounds, data.data(), static_cast<int>(data.size()), editable_);
    } else {
      data.copy(buffer_.data(), buffer_.size());

      GuiTextBox(bounds, buffer_.data(), static_cast<int>(buffer_.size()), editable_);

      data = std::string(buffer_.data());
    }
}
