#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

#include "game_data.hpp"
#include "state.hpp"
#include "tile_map.hpp"

struct GameState final : public State<GameData> {
  entt::registry world;
  raylib::Camera2D camera;
  TileMap& tmap;

  GameState(raylib::Window&, TileMap&);

  auto update(GameData game) -> void override;
  auto draw() -> void override;
};

#endif /* GAME_STATE_HPP */
