#define RAYGUIDEF extern "C"
#include "SliderField.hh"
#include <charconv>
#include "raygui.h"

SliderField::SliderField(
  Rectangle &parent,
  std::string const & minLabel,
  std::string const & maxLabel,
  float minValue,
  float maxValue)
        : sliderMinText_(minLabel)
        , sliderMaxText_(maxLabel)
        , minSlider_(minValue)
        , maxSlider_(maxValue)
        , parent_(parent) {}


void SliderField::render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex) {
    Rectangle sliderBounds = {parent_.x + 10, parent_.y + (35 * stackIndex), 120, 25};
    Rectangle valueBounds = sliderBounds;

    sliderBounds.x += 220;
    valueBounds.x += 60;

    if (mousepressed) {
        if (CheckCollisionPointRec(mousepos, valueBounds)) {
            fieldEditable_ = true;
        } else {
            fieldEditable_ = false;
        }
    }

    data = GuiSlider(sliderBounds, sliderMinText_.c_str(), sliderMaxText_.c_str(), data, minSlider_, maxSlider_);
    //GuiLabel(labelBounds, label_.c_str());

    if (!fieldEditable_) {
        buffer_.fill('\0');
        std::to_chars(buffer_.data(), buffer_.data() + buffer_.size(), data);
    }
    GuiTextBox(valueBounds, buffer_.data(), buffer_.size(), fieldEditable_);

    if ( fieldEditable_ ) {
        float value = 0.0f;
        auto [ptr, ec] = std::from_chars(buffer_.data(), buffer_.data() + buffer_.size(), value);
        if (ec == std::errc())
        {
            data = value;
        }
    }
}