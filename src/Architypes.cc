
#include "Architypes.hh"
#include "Components.hh"


entt::entity Architypes::createLayer(entt::registry &registry, std::string const &name) {
  auto entity = registry.create();
  registry.emplace<Components::Name>(entity, name);
  registry.emplace<Components::ZIndex>(entity, 0);
  registry.emplace<Components::TileTextures>(entity);
  registry.emplace<Components::Tiles>(entity);
  return entity;
}
