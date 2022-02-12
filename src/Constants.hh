
#pragma once

#include "raylib.h"

namespace Constants {
  static const float screenHeight = 650.f;
  static const float screenWidth = 900.f;

  static const Rectangle screenRectangle { 0, 0, screenWidth, screenHeight };

  static const int debugScreenHeight = static_cast<const int>(screenHeight + 200.f);
  static const int debugScreenWidth = static_cast<const int>(screenWidth + 600.f);
}// namespace Constants
