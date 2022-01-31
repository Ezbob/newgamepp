
#pragma once

#include "Components.hh"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "IEnttModel.hh"

struct TileModel : public IEnttModel {
  Texture texture;
  int zIndex = 1;
  int layerIndex = 1;
  float alpha = 1.0f;
  Vector2 position = {0, 0};
  Rectangle quad = {0, 0, 0, 0};
  bool vFlip = false;
  bool hFlip = false;

  inline void reset() {
    texture = {0};
    zIndex = 1;
    layerIndex = 1;
    alpha = 1.0f;
    position = {0, 0};
    quad = {0, 0, 0, 0};
    vFlip = false;
    hFlip = false;
  }

  inline entt::entity create(entt::registry &reg) const override {
    entt::entity entity = reg.create();
    reg.emplace<Components::SpriteTexture>(entity, texture);
    reg.emplace<Components::Renderable>(entity, alpha, zIndex, layerIndex);
    reg.emplace<Components::Position>(entity, position.x, position.y);
    reg.emplace<Components::Flipable>(entity, hFlip, vFlip);
    reg.emplace<Components::Quad>(entity, quad);
    return entity;
  };

  inline void update(entt::registry &reg, entt::entity e) const override {
    if (!reg.valid(e)) return;

    reg.get<Components::SpriteTexture>(e).texture = texture;

    auto &render = reg.get<Components::Renderable>(e);
    render.alpha = alpha;
    render.layer = layerIndex;
    render.zIndex = zIndex;

    auto &flip = reg.get<Components::Flipable>(e);
    flip.hFlipped = hFlip;
    flip.vFlipped = vFlip;

    auto &pos = reg.get<Components::Position>(e);
    pos.x = position.x;
    pos.y = position.y;

    reg.get<Components::Quad>(e).quad = quad;
  }

  inline entt::entity read(entt::registry &reg, entt::entity e) override {
    if (!reg.valid(e)) return e;

    texture = reg.get<Components::SpriteTexture>(e).texture;

    auto const&render = reg.get<Components::Renderable>(e);
    alpha = render.alpha;
    layerIndex = render.layer;
    zIndex = render.zIndex;

    auto const&flip = reg.get<Components::Flipable>(e);
    hFlip = flip.hFlipped;
    vFlip = flip.vFlipped;

    auto &pos = reg.get<Components::Position>(e);
    position.x = pos.x;
    position.y = pos.y;

    quad = reg.get<Components::Quad>(e).quad;
    return e;
  }

  inline void destroy(entt::registry &reg, entt::entity e) override {
    if (!reg.valid(e)) return;
    reg.destroy(e);
  }
};