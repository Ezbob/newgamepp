
#pragma once

#include "IEnttModel.hh"
#include "raylib.h"

struct SelectionBoxModel : public IEnttModel {
  Rectangle boundary = {0};
  bool show = false;

  inline entt::entity create(entt::registry &reg) const override {
    entt::entity entity = reg.create();
    reg.emplace<Components::Quad>(entity, boundary);
    reg.emplace<Components::Active>(entity, show);
    reg.emplace<Components::Debug>(entity);

    return entity;
  }

  inline void update(entt::registry &reg, entt::entity e) const override {
    if (!reg.valid(e)) return;
    reg.get<Components::Quad>(e).quad = boundary;
    reg.get<Components::Active>(e).isActive = show;
  }

  inline entt::entity read(entt::registry &reg, entt::entity e) override {
    if (!reg.valid(e)) return entt::null;
    boundary = reg.get<Components::Quad>(e).quad;
    show = reg.get<Components::Active>(e).isActive;

    return e;
  }

};