
#include "PainterTool.hh"
#include "Components.hh"
#include "Constants.hh"
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"


PainterTool::PainterTool(
        entt::registry &reg,
        TileSetSelector &selector,
        Camera2D &camera,
        SelectedCollection &selectionTiles,
        int &currentLayer)
    : registry_(reg), selector_(selector), camera_(camera), selectedTiles_(selectionTiles), currentLayer_(currentLayer) {
}

int PainterTool::roundDownTo(int N, int n) const {
  assert(N > 0);
  // Smaller multiple
  int a = (n / N) * N;

  // Larger multiple
  int b = a + N;

  // Return of closest of two
  return (n - a > b - n) ? b : a;
}

Vector2 PainterTool::midPoint(Rectangle r) const {
  return {r.x - (r.width / 2), r.y - (r.height / 2)};
}

void PainterTool::renderToolAttributes(Rectangle const& position) {

  if (selectedTiles_.size() == 0) GuiDisable();

  float alpha = 1.f;
  bool vFlip = false;
  bool hFlip = false;

  bool matches = true;

  if (selectedTiles_.size() > 0) {
    auto first = *selectedTiles_.begin();
    auto &flipFirst = registry_.get<Components::Flipable>(first);
    auto &renderFirst = registry_.get<Components::Renderable>(first);

    alpha = renderFirst.alpha;
    vFlip = flipFirst.vFlipped;
    hFlip = flipFirst.hFlipped;
  }

  GuiSpinnerEx({position.x + 55.f, position.y + 10.f, 125.f, 20.f}, "Alpha:", &alpha, 0.f, 1.f, 0.01f, false);

  auto oldTextAlign = GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT);
  GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

  hFlip = GuiCheckBox({position.x + 90.f, position.y + 10.f + (25.f * 1.f), 20.f, 20.f}, "Horizontal Flip:", hFlip);

  vFlip = GuiCheckBox({position.x + 90.f, position.y + 10.f + (25.f * 2.f), 20.f, 20.f}, "Vertical Flip:", vFlip);

  GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, oldTextAlign);

  if (selectedTiles_.size() > 0) {
    auto e = *selectedTiles_.begin();
    auto &flipable = registry_.get<Components::Flipable>(e);
    flipable.hFlipped = hFlip;
    flipable.vFlipped = vFlip;
    auto &renderable = registry_.get<Components::Renderable>(e);
    renderable.alpha = alpha;
  }

  if (selectedTiles_.size() == 0) GuiEnable();
}


void PainterTool::execute() {
  auto mousePosition = GetMousePosition();
  if (CheckCollisionPointRec(mousePosition, Constants::screenRectangle)) {
    auto selectedTileSet_ = selector_.getSelectedFrame();

    auto &tileFrame = selectedTileSet_->set.frames[selectedTileSet_->frameIndex];
    auto frame = tileFrame.frameDimensions;

    auto midPointMouse = midPoint({mousePosition.x,
                                   mousePosition.y,
                                   tileFrame.frameDimensions.width,
                                   tileFrame.frameDimensions.height});

    midPointMouse = GetScreenToWorld2D(midPointMouse, camera_);

    if (!IsKeyDown(KEY_LEFT_SHIFT)) {
      // snapping to tile width/height
      float roundedX = (float) roundDownTo(static_cast<int>(frame.width), abs(static_cast<int>(midPointMouse.x)));
      float roundedY = (float) roundDownTo(static_cast<int>(frame.height), abs(static_cast<int>(midPointMouse.y)));

      midPointMouse.x = midPointMouse.x < 0.f ? -roundedX : roundedX;
      midPointMouse.y = midPointMouse.y < 0.f ? -roundedY : roundedY;
    }


    BeginMode2D(camera_);
    DrawTextureRec(
            selectedTileSet_->set.texture,
            frame,
            midPointMouse,
            ColorAlpha(WHITE, 0.6f));
    EndMode2D();

    if (IsMouseButtonPressed(0)) {
      entt::entity entity = registry_.create();

      registry_.emplace<Components::SpriteTexture>(entity, selectedTileSet_->set.texture);
      registry_.emplace<Components::Renderable>(entity, 1.f, currentLayer_);
      registry_.emplace<Components::Position>(entity, midPointMouse.x, midPointMouse.y);
      registry_.emplace<Components::Flipable>(entity);
      registry_.emplace<Components::Quad>(entity, tileFrame.frameDimensions);

      selectedTiles_.clear();
      selectedTiles_.insert(entity);
    }
  }
}