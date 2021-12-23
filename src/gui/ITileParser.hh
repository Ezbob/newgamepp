
#pragma once

#include <variant>
#include <string>
#include "TileSet.hh"

class ITileParser {
  public:
    virtual ~ITileParser() = default;
  
  struct TileParseFault {
    std::string description;
    size_t lineno;
    size_t charno;
  };

  virtual std::variant<TileSet, TileParseFault> parse(std::string const &filename) = 0;

  virtual const char *getFileExtensions() const = 0;
};