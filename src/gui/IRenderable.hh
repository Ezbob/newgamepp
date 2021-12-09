
#pragma once

struct IRenderable {
  virtual ~IRenderable() = default;
  virtual bool render() = 0;
};