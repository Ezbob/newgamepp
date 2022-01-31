
#pragma once

#include "Components.hh"
#include "entt/entity/registry.hpp"
#include "IEnttModel.hh"
#include "Constants.hh"

struct GridModel : public IEnttModel {
    Color color = GRAY;
    bool show = false;
    Vector2 dimensions = {Constants::screenWidth * 4.f, Constants::screenHeight};

    inline void reset() override {
      color = GRAY;
      show = false;
      dimensions = {Constants::screenWidth * 4.f, Constants::screenHeight};
    }

    inline entt::entity create(entt::registry &r) const override {
      entt::entity entity = r.create();
      r.emplace<Components::Coloring>(entity, color);
      r.emplace<Components::Debug>(entity);
      r.emplace<Components::Active>(entity, show);
      r.emplace<Components::Dimensions>(entity, dimensions.x, dimensions.y);
      return entity;
    }

    inline void destroy(entt::registry &r, entt::entity e) override {
      if (!r.valid(e)) return;
      r.destroy(e);
    }

    inline entt::entity read(entt::registry &r, entt::entity e) override {
      if (!r.valid(e)) return e;
      color = r.get<Components::Coloring>(e).color;
      show = r.get<Components::Active>(e).isActive;
      return e;
    }

    inline void update(entt::registry &r, entt::entity e) const override {
      if (!r.valid(e)) return;
      r.get<Components::Coloring>(e).color = color;
      r.get<Components::Active>(e).isActive = show;
    }
};