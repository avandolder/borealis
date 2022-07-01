#include <algorithm>

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

#include "game_data.hpp"
#include "game_state.hpp"
#include "pause_state.hpp"

namespace rl = raylib;

struct Pos {
  float x;
  float y;
};

struct Vel {
  float x;
  float y;
};

GameState::GameState(rl::Window& window, TileMap& tmap)
    : camera(window.GetSize() / 2.f, tmap.size() / 2.f, 0.0f, 2.0f),
      tmap(tmap) {
  for (auto i = 0u; i < 10u; ++i) {
    const auto entity = world.create();
    world.emplace<Pos>(entity, i * 1.f, i * 1.f);
    if (i % 2 == 0) {
      world.emplace<Vel>(entity, i * 1.f, i * 1.f);
    }
  }
}

auto GameState::update(GameData game) -> void {
  auto [sm, _, window] = game;
  const auto dt = window.GetFrameTime();

  world.view<Pos, Vel>().each([=](auto& pos, auto& vel) {
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
  });

  if (IsKeyPressed(KEY_SPACE)) {
    sm.push(std::make_unique<PauseState>());
  } else if (IsKeyPressed(KEY_ENTER)) {
    sm.pop();
  }

  rl::Vector2 delta{IsKeyDown(KEY_LEFT)    ? -1.f
                    : IsKeyDown(KEY_RIGHT) ? 1.f
                                           : 0.f,
                    IsKeyDown(KEY_UP)     ? -1.f
                    : IsKeyDown(KEY_DOWN) ? 1.f
                                          : 0.f};
  camera.target =
      rl::Vector2(camera.target) + delta.Normalize() * 200 * dt;

  const auto tmap_size = tmap.size();
  camera.target = rl::Vector2(
      std::clamp(camera.target.x, camera.offset.x / camera.zoom,
                 tmap_size.x - camera.offset.x / camera.zoom),
      std::clamp(camera.target.y, camera.offset.y / camera.zoom,
                 tmap_size.y - camera.offset.y / camera.zoom));
}

auto GameState::draw() -> void {
  camera.BeginMode();

  tmap.draw(camera);

  world.view<Pos>().each([](const auto& pos) {
    DrawRectangle(pos.x, pos.y, 1, 1, rl::WHITE);
  });

  camera.EndMode();
}
