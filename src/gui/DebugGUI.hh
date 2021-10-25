
#pragma once


#include "EntityChooser.hh"
#include "Components.hh"
#include <optional>
#include "entt/entity/registry.hpp"

class DebugGUI {

private:

    EntityChooser entityChooser_;
    entt::registry &registry_;


    std::optional<entt::entity> findEntity(entt::registry &registry, Vector2 &mousePos);
public:
    DebugGUI(entt::registry &);

    void doGui();
};

