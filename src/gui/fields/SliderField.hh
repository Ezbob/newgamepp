
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
      float minValue = -300.f,
      float maxValue = 300.f,
      std::string minLabel = "Min",
      std::string maxLabel = "Max"
    );

    void render(bool mousepressed, Vector2 &mousepos, float &data, int stackIndex = 1);

private:
    std::string sliderMinText_;
    std::string sliderMaxText_;
    float minSlider_;
    float maxSlider_;
    Rectangle &parent_;
    Rectangle bounds_;
};