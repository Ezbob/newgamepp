
#pragma once

#include <stdint.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"


struct TileMap {
  struct TilePosition {
    uint32_t x;
    uint32_t y;
    uint32_t fx;
    uint32_t fy;
    uint32_t w;
    uint32_t h;

    float alpha;
    uint32_t layer;

    uint32_t tileSetIndex;

    template<typename Archive>
    void serialize(Archive &ar) {
      ar(x, y, fx, fy, w, h, alpha, layer, tileSetIndex);
    }
  };

  std::vector<std::string> tileSetLocations;
  std::vector<TilePosition> tilePositions;

  template<typename Archive>
  void serialize(Archive &ar) {
    ar(tileSetLocations, tilePositions);
  }

  void save(std::filesystem::path const& p);
};

