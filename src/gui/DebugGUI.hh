
#pragma once

#include "EntityChooser.hh"
#include "Components.hh"
#include <optional>
#include "entt/entity/registry.hpp"

class DebugGUI {

private:
    EntityChooser entityChooser_;
    bool drawWindow_ = false;

public:
    explicit DebugGUI(entt::registry &);

    void doGui();
};

