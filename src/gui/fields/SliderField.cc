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
        , parent_(parent)
        , valueField_(parent, {
            70, 35, 120, 25
        }) {}

void SliderField::render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex) {
    Rectangle sliderBounds = {parent_.x + 230, parent_.y + (35 * stackIndex), 120, 25};

    data = GuiSlider(sliderBounds, sliderMinText_.c_str(), sliderMaxText_.c_str(), data, minSlider_, maxSlider_);

    valueField_.render(mousepressed, mousepos, data, stackIndex);
}