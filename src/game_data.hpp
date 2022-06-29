#ifndef GAME_DATA_HPP
#define GAME_DATA_HPP

#include "state_manager.hpp"
#include "tile_map_manager.hpp"

struct Config {
  int width;
  int height;
  int fps;

  static const int DEFAULT_WIDTH = 1280;
  static const int DEFAULT_HEIGHT = 720;
  static const int DEFAULT_FPS = 0;
};

struct GameData final {
  StateManager<GameData>& sm;
  TileMapManager& tm;
  Config& config;
};

#endif /* GAME_DATA_HPP */
