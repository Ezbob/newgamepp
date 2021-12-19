
#pragma once

#include "ITileParser.hh"
#include <functional>
#include "nlohmann/json.hpp"


class AsepriteParser : public ITileParser
{
public:
  struct LabelSplit {
    std::string frame_label;
    size_t index;
  };

  using LabelSplitter_t = std::function<LabelSplit(std::string const &)>;

  AsepriteParser(LabelSplitter_t splitter = defaultSplitter);

  std::variant<TileSet, TileParseFault> parse(std::string const &filename);

private:
  static LabelSplit defaultSplitter(std::string const&raw);

  bool isValidRecord(nlohmann::json const &) const;
  LabelSplitter_t label_parser_;
};
