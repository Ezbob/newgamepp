
#pragma once

#include "EntityWindow.hh"

class DebugGUI {

private:
    EntityWindow entityWindow_;

public:
    DebugGUI(entt::registry &);

    void doGui();
};

