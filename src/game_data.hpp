#ifndef GAME_DATA_HPP
#define GAME_DATA_HPP

#include <raylib-cpp.hpp>

#include "state_manager.hpp"
#include "tile_map_manager.hpp"

struct GameData final {
  StateManager<GameData>& sm;
  TileMapManager& tm;
  raylib::Window& window;
};

#endif /* GAME_DATA_HPP */
