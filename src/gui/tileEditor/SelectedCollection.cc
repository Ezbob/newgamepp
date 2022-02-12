
#include "SelectedCollection.hh"
#include "Components.hh"

SelectedCollection::SelectedCollection(entt::registry &r, std::vector<entt::entity> const& es) 
  : selected_(es.begin(), es.end())
  , registry_(r) {}

SelectedCollection::SelectedCollection(entt::registry &r)
  : registry_(r) {}

void SelectedCollection::clear() {
  for (auto &e : selected_) {
    if (registry_.valid(e)) {
      registry_.remove<Components::Debug>(e);
    }
  }
  selected_.clear();
}

void SelectedCollection::destroy_all() {
  for (auto &e : selected_) {
    if (registry_.valid(e)) {
      registry_.destroy(e);
    }
  }
  selected_.clear();
}

void SelectedCollection::remove(entt::entity e) {
  if (!registry_.all_of<Components::Debug>(e)) {
    return;
  }
  registry_.remove<Components::Debug>(e);
  for (auto it = selected_.begin(); it != selected_.end(); ++it ) {
    if (*it == e) {
      selected_.erase(it);
      break;
    }
  }
}

void SelectedCollection::insert(entt::entity e) {
  if (!registry_.all_of<Components::Debug>(e)) {
    registry_.emplace<Components::Debug>(e);
  }
  selected_.push_back(e);
}
