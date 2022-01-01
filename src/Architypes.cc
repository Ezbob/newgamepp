
#include "Architypes.hh"
#include "Components.hh"


entt::entity Architypes::createLayer(entt::registry &registry) {
  auto entity = registry.create();
  registry.emplace<Components::TileTexture>(entity);
  registry.emplace<Components::Tiles>(entity);
  return entity;
}
