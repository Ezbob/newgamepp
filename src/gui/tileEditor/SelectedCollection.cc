
#include "SelectedCollection.hh"
#include "Components.hh"

SelectedCollection::SelectedCollection(entt::registry &r, std::vector<entt::entity> const& es) 
  : selected_(es.begin(), es.end())
  , registry_(r) {}

SelectedCollection::SelectedCollection(entt::registry &r)
  : registry_(r) {}

void SelectedCollection::clear() {
  for (auto &e : selected_) {
    registry_.remove<Components::Debug>(e);
  }
  selected_.clear();
}

void SelectedCollection::remove(entt::entity e) {
  if (!registry_.all_of<Components::Debug>(e)) {
    return;
  }
  for (auto it = selected_.begin(); it != selected_.end(); ++it ) {
    if (*it == e) {
      selected_.erase(it);
      break;
    }
  }
}

void SelectedCollection::push_back(entt::entity e) {
  if (!registry_.all_of<Components::Debug>(e)) {
    registry_.emplace<Components::Debug>(e);
  }
  selected_.push_back(e);
}

SelectedCollection::reference SelectedCollection::at(SelectedCollection::size_type i) {
  return selected_.at(i);
}

SelectedCollection::const_reference SelectedCollection::at(SelectedCollection::size_type i) const {
  return selected_.at(i);
}

SelectedCollection::reference SelectedCollection::operator[](SelectedCollection::size_type i) {
  return selected_[i];
}

SelectedCollection::const_reference SelectedCollection::operator[](SelectedCollection::size_type i) const {
  return selected_[i];
}

void SelectedCollection::pop_back() noexcept {
  auto e = selected_.back();
  if (!registry_.all_of<Components::Debug>(e)) {
    registry_.emplace<Components::Debug>(e);
  }
  return selected_.pop_back();
}