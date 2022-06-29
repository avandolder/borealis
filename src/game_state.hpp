#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <entt/entt.hpp>

#include "game_data.hpp"
#include "raylib.hpp"
#include "state.hpp"
#include "tile_map.hpp"

struct GameState final : public State<GameData> {
  entt::registry world;
  TileMap& tmap;
  rl::Camera2D camera;

  GameState(TileMap&);

  auto update(GameData& game) -> void override;
  auto draw() -> void override;
};

#endif /* GAME_STATE_HPP */
