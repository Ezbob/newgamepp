
#pragma once

#include <stdint.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/archives/binary.hpp"

#include "TileSet.hh"

struct TileMap {
  struct TilePosition {
    int64_t x;
    int64_t y;

    float alpha;
    uint32_t layer;

    bool vFlipped = false;
    bool hFlipped = false;

    uint32_t tileSetIndex;
    uint32_t tileFrameIndex;

    template<typename Archive>
    void serialize(Archive &ar) {
      ar(x, y, alpha, layer, tileSetIndex, tileFrameIndex, vFlipped, hFlipped);
    }
  };

  uint32_t maxLayer;

  std::vector<TileSet> tileSetLocations;
  std::vector<TilePosition> tilePositions;

  template<typename Archive>
  void serialize(Archive &ar) {
    ar(maxLayer, tileSetLocations, tilePositions);
  }

  template<typename Archive = cereal::BinaryOutputArchive>
  void save(std::filesystem::path const& p) {
    std::ofstream file(p);
    Archive ar(file);
    ar(*this);
  }

  template<typename Archive = cereal::BinaryInputArchive>
  void load(std::filesystem::path const& p) {
    std::ifstream file(p);
    Archive ar(file);
    ar(*this);
  }
};

