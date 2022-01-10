
#include "Archetypes.hh"
#include "Components.hh"


entt::entity Archetypes::createTile(entt::registry &registry, Texture const &texture, Vector2 pos, Rectangle quad, float alpha, int zIndex, int layerIndex) {
  auto entity = registry.create();
  registry.emplace<Components::SpriteTexture>(entity, texture);
  registry.emplace<Components::Renderable>(entity, alpha, zIndex, layerIndex);
  registry.emplace<Components::Position>(entity, pos.x, pos.y);
  registry.emplace<Components::Quad>(entity, quad);
  return entity;
}
