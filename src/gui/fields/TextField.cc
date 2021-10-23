
#include "TextField.hh"
#include <charconv>
#include "raygui.h"

TextField::TextField(Rectangle &parent, Rectangle bounds)
  : parent_(parent)
  , bounds_(bounds)
  {}

void TextField::render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex) {
    Rectangle bounds = {
      parent_.x + bounds_.x,
      parent_.y + bounds_.y * stackIndex,
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
        buffer_.fill('\0');
        std::to_chars(buffer_.data(), buffer_.data() + buffer_.size(), data);
    }
    GuiTextBox(bounds, buffer_.data(), buffer_.size(), editable_);

    if ( editable_ ) {
        float value = 0.0f;
        auto [ptr, ec] = std::from_chars(buffer_.data(), buffer_.data() + buffer_.size(), value);
        if (ec == std::errc())
        {
            data = value;
        }
    }
}

void TextField::render(bool mousepressed, Vector2 &mousepos, int &data, int stackIndex) {
    Rectangle bounds = {
      parent_.x + bounds_.x,
      parent_.y + bounds_.y * stackIndex,
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
        buffer_.fill('\0');
        std::to_chars(buffer_.data(), buffer_.data() + buffer_.size(), (float)data);
    }
    GuiTextBox(bounds, buffer_.data(), buffer_.size(), editable_);

    if ( editable_ ) {
        float value = 0.0f;
        auto [ptr, ec] = std::from_chars(buffer_.data(), buffer_.data() + buffer_.size(), value);
        if (ec == std::errc())
        {
            data = (int)value;
        }
    }
}