
#pragma once

#include "entt/entity/registry.hpp"
#include <vector>

/**
 * Like a vector of entities, but it also removes the selection component
 * from the entity
 */
class SelectedCollection {
private:
  std::vector<entt::entity> selected_;
  entt::registry &registry_;
public:
  SelectedCollection(entt::registry &, std::vector<entt::entity> const&);
  SelectedCollection(entt::registry &);

  using value_type = std::vector<entt::entity>::value_type;
  using size_type = std::vector<entt::entity>::size_type;
  using reference = std::vector<entt::entity>::reference;
  using const_reference = std::vector<entt::entity>::const_reference;
  using iterator = std::vector<entt::entity>::iterator;
  using const_iterator = std::vector<entt::entity>::const_iterator;

  /**
   * Clears the selection container of selected entities.
   * This does not remove entities from the registry but removes the selected marker component
   * on all entities entities stored before clearing the internal storage
   */
  void clear();

  /**
   * Clears the selection container AND removes the entities from the registry
   */
  void destroy_all();

  /**
   * Removes the entity from the collection and it's selection component
   */
  void remove(entt::entity);

  /**
   * Inserts the entity into the collection
   */
  void insert(entt::entity);

  inline size_type size() const {
    return selected_.size();
  };

  inline iterator begin() noexcept {
    return selected_.begin();
  }

  inline iterator end() noexcept {
    return selected_.end();
  }

  inline std::reverse_iterator<iterator> rbegin() noexcept {
    return selected_.rbegin();
  }

  inline std::reverse_iterator<iterator> rend() noexcept {
    return selected_.rend();
  }

  inline std::reverse_iterator<const_iterator> crbegin() const noexcept {
    return selected_.crbegin();
  }

  inline std::reverse_iterator<const_iterator> crend() const noexcept {
    return selected_.crend();
  }

  inline const_iterator cbegin() const noexcept {
    return selected_.cbegin();
  }

  inline const_iterator cend() const noexcept {
    return selected_.cend();
  }
};