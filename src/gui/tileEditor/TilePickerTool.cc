
#include "TilePickerTool.hh"
#include "raylib.h"
#include "TilePicker.hh"
#include "Components.hh"
#include "Constants.hh"

TilePickerTool::TilePickerTool(entt::registry &r, Camera2D &c, int &layer, entt::entity &e, TileModel &tm)
  : registry_(r)
  , selectedTile_(e)
  , camera_(c)
  , currentLayerId_(layer)
  , tileModel_(tm)
{

}

void TilePickerTool::execute() {
  if (IsMouseButtonPressed(0)) {
    auto mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {
      mousePosition = GetScreenToWorld2D(mousePosition, camera_);
      if (auto found = TilePicker::findClickedTile(registry_, mousePosition, currentLayerId_); registry_.valid(found)) {
        if (registry_.valid(selectedTile_) && registry_.all_of<Components::Debug>(selectedTile_)) {
          registry_.remove<Components::Debug>(selectedTile_);
        }
        selectedTile_ = tileModel_.read(registry_, found);

        registry_.emplace<Components::Debug>(selectedTile_);
      }
    }
  }
}