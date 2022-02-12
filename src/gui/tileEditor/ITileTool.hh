
#pragma once


class ITileTool {
public:
  virtual ~ITileTool() = default;
  virtual void execute() = 0;

  virtual void onSelected() {};
  virtual void renderToolAttributes(Rectangle const&) {}
};