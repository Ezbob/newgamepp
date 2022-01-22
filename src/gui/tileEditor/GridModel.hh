
#pragma once

#include "Components.hh"
#include "entt/entity/registry.hpp"
#include "IEnttModel.hh"
#include "Constants.hh"

struct GridModel : public IEnttModel {
    Color color = GRAY;
    bool show = false;

    inline void reset() override {
      color = GRAY;
      show = false;
    }

    inline entt::entity create(entt::registry &r) const override {
      entt::entity entity = r.create();
      r.emplace<Components::Coloring>(entity, color);
      r.emplace<Components::Debug>(entity);
      r.emplace<Components::Active>(entity, show);
      r.emplace<Components::Position>(entity, 0.f, 0.f);
      r.emplace<Components::Dimensions>(entity, Constants::screenWidth, Constants::screenHeight);
      return entity;
    }

    inline void destroy(entt::registry &r, entt::entity e) override {
      r.destroy(e);
    }

    inline entt::entity read(entt::registry &r, entt::entity e) override {
      color = r.get<Components::Coloring>(e).color;
      show = r.get<Components::Active>(e).isActive;
      return e;
    }

    inline void update(entt::registry &r, entt::entity e) const override {
      r.get<Components::Coloring>(e).color = color;
      r.get<Components::Active>(e).isActive = show;
    }
};