
#include <fstream>
#include "TileMap.hh"
#include "cereal/archives/binary.hpp"

void TileMap::save(std::filesystem::path const& p) {
  std::ofstream file(p);
  cereal::BinaryOutputArchive ar(file);
  ar(*this);
}