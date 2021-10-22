
#include "DebugGUI.hh"

DebugGUI::DebugGUI(entt::registry &r) : entityWindow_(r) {}

void DebugGUI::doGui() {
    entityWindow_.doGui();
}