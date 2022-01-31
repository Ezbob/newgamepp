
#pragma once
#include "ITileTool.hh"

struct NullTileTool : public ITileTool {
  inline void execute() {}
};