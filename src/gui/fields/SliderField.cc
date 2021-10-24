#include "SliderField.hh"
#include <charconv>
#include "raygui.h"

SliderField::SliderField(
  Rectangle &parent,
  Rectangle bounds,
  float minValue,
  float maxValue,
  std::string const & minLabel,
  std::string const & maxLabel)
        : bounds_(bounds)
        , sliderMinText_(minLabel)
        , sliderMaxText_(maxLabel)
        , minSlider_(minValue)
        , maxSlider_(maxValue)
        , parent_(parent)
        {}

void SliderField::render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex) {
    Rectangle sliderBounds = {
        parent_.x + bounds_.x,
        parent_.y + (bounds_.y * stackIndex),
        bounds_.width,
        bounds_.height
    };

    data = GuiSlider(sliderBounds, sliderMinText_.c_str(), sliderMaxText_.c_str(), data, minSlider_, maxSlider_);
}