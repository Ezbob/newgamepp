#pragma once

#include "entt/entity/registry.hpp"

struct IEnttModel {
  virtual ~IEnttModel() = default;
  virtual void reset() {};
  virtual entt::entity create(entt::registry &) const = 0;
  virtual void update(entt::registry &, entt::entity) const = 0;
  virtual entt::entity read(entt::registry &, entt::entity) = 0;
  virtual inline void destroy(entt::registry &reg, entt::entity e) {
    if (!reg.valid(e)) return;
    reg.destroy(e);
  };
};