
#include "DebugGUI.hh"
#include "raylib.h"

DebugGUI::DebugGUI(entt::registry &r) : entityChooser_(r), registry_(r) {}

void DebugGUI::doGui() {

    entityChooser_.render();
}

