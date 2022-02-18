
#pragma once

#include <stdint.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"

#include "TileSet.hh"

struct TileMap {
  struct TilePosition {
    uint32_t x;
    uint32_t y;

    float alpha;
    uint32_t layer;

    uint32_t tileSetIndex;
    uint32_t tileFrameIndex;

    template<typename Archive>
    void serialize(Archive &ar) {
      ar(x, y, alpha, layer, tileSetIndex, tileFrameIndex);
    }
  };

  std::vector<TileSet> tileSetLocations;
  std::vector<TilePosition> tilePositions;

  template<typename Archive>
  void serialize(Archive &ar) {
    ar(tileSetLocations, tilePositions);
  }

  void save(std::filesystem::path const& p);
};

