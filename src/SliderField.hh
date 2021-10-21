
#pragma once

#include "raylib.h"
#include <string>
#include <array>

class SliderField {

public:
    SliderField(
      std::string const & label,
      std::string const & minLabel = "Min",
      std::string const & maxLabel = "Max",
      float minValue = -300.f, float maxValue = 300.f);

    void render(int stackIndex, Rectangle &parent, bool mousepressed, Vector2 &mousepos, float &data);
private:
    bool fieldEditable_ = false;
    std::array<char, 16> buffer_;
    std::string label_;
    std::string sliderMinText_;
    std::string sliderMaxText_;
    float minSlider_;
    float maxSlider_;
};