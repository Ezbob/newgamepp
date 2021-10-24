
#include "DebugGUI.hh"
#include "raylib.h"

DebugGUI::DebugGUI(entt::registry &r) : entityWindow_(r), registry_(r) {}

void DebugGUI::doGui() {

    bool mousepressed = IsMouseButtonPressed(0);

    if (mousepressed) {
        Vector2 mousepos = GetMousePosition();
        auto entityFound = findEntity(registry_, mousepos);
        if (entityFound) {
            selected_ = entityFound;
        }
    }

    if (selected_) {
        bool isNotClosed = entityWindow_.doGui(selected_.value());
        if (!isNotClosed) {
            selected_ = std::nullopt;
        }
    }
}


std::optional<entt::entity> DebugGUI::findEntity(entt::registry &registry, Vector2 &mousePos) {
    auto view = registry.view<Position, Dimensions>();

    for (auto ent : view) {
        auto const &pos = view.get<Position>(ent);
        auto const &dim = view.get<Dimensions>(ent);
        Rectangle entityBox {
            (float)(pos.x),
            (float)(pos.y),
            (float)(dim.w),
            (float)(dim.h)
        };
        if (CheckCollisionPointRec(mousePos, entityBox)) {
            return ent;
        }
    }
    return std::nullopt;
}