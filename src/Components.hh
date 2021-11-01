
#pragma once

#include <string>

namespace Components {

    struct Position {
        float x;
        float y;
    };

    struct Dimensions {
        float w;
        float h;
    };

    struct Velocity {
        float dx;
        float dy;
    };

    struct Name {
        std::string name;
    };

    struct ScreenClamp {
      float left = 0.2f;
      float right = 0.2f;
      float bottom = 0.2f;
      float top = 0.2f;
    };

};