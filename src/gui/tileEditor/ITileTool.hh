
#pragma once


class ITileTool {
public:
  virtual ~ITileTool() = default;
  virtual void execute() = 0;
};