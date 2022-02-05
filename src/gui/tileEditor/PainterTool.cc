
#include "PainterTool.hh"
#include "raylib.h"
#include "Constants.hh"
#include "raymath.h"


PainterTool::PainterTool(
    entt::registry &reg,
    TileSelector &selector,
    TileModel &model,
    Camera2D &camera,
    entt::entity &selectedTile) 
  : registry_(reg)
  , selector_(selector)
  , tileModel_(model)
  , camera_(camera)
  , selectedTile_(selectedTile) {
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


void PainterTool::execute() {
  auto mousePosition = GetMousePosition();
  if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {
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


    if (IsKeyDown(KEY_LEFT_CONTROL)) {
      frame.width = tileModel_.vFlip ? -frame.width : frame.width;
      frame.height = tileModel_.hFlip ? -frame.height : frame.height;
    }

    BeginMode2D(camera_);
    DrawTextureRec(
            selectedTileSet_->set.texture,
            frame,
            midPointMouse,
            ColorAlpha(WHITE, 0.6f));
    EndMode2D();

    if (IsMouseButtonPressed(0)) {

      if (!IsKeyDown(KEY_LEFT_CONTROL)) {
        tileModel_.reset();
      }

      if (registry_.valid(selectedTile_) && registry_.all_of<Components::Debug>(selectedTile_)) {
        registry_.remove<Components::Debug>(selectedTile_);
      }

      selectedTile_ = tileModel_.create(registry_);

      registry_.emplace<Components::Debug>(selectedTile_);

      tileModel_.texture = selectedTileSet_->set.texture;
      tileModel_.position = midPointMouse;

      tileModel_.quad = tileFrame.frameDimensions;
    }
  }
}