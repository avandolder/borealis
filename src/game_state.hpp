#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

#include "game_data.hpp"
#include "state.hpp"
#include "tile_map.hpp"

struct GameState final : public State<GameData> {
 private:
  entt::registry world_;
  raylib::Camera2D camera_;
  TileMap& tmap_;

 public:
  GameState(raylib::Window&, TileMap&);

  void update(GameData game) override;
  void draw() override;
};

#endif /* GAME_STATE_HPP */
