
#include "TilePickerTool.hh"
#include "raylib.h"
#include "TilePicker.hh"
#include "Components.hh"
#include "Constants.hh"

TilePickerTool::TilePickerTool(entt::registry &r, Camera2D &c, int &layer, std::vector<entt::entity> &tt)
  : registry_(r)
  , selectedTiles_(tt)
  , camera_(c)
  , currentLayerId_(layer)
{

}

void TilePickerTool::execute() {
  if (IsMouseButtonPressed(0)) {
    auto mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, {0, 0, Constants::screenWidth, Constants::screenHeight})) {
      mousePosition = GetScreenToWorld2D(mousePosition, camera_);
      if (auto found = TilePicker::findClickedTile(registry_, mousePosition, currentLayerId_); registry_.valid(found)) {
        for (entt::entity et : selectedTiles_) {
          if (registry_.valid(et) && registry_.all_of<Components::Debug>(et)) {
            registry_.remove<Components::Debug>(et);
          }
        }
        selectedTiles_.push_back(found);
        registry_.emplace<Components::Debug>(found);
      }
    }
  }
}