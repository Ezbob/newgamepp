
#pragma once

#include "raylib.h"
#include <string>
#include <array>
#include "TextField.hh"

class SliderField {

public:
    SliderField(
      Rectangle &parent,
      Rectangle bounds,
      std::string const& minLabel = "Min",
      std::string const& maxLabel = "Max",
      float minValue = -300.f,
      float maxValue = 300.f);

    void render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex = 1);

private:
    std::string sliderMinText_;
    std::string sliderMaxText_;
    float minSlider_;
    float maxSlider_;
    Rectangle &parent_;
    Rectangle bounds_;
};