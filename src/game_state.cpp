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
    : camera_(window.GetSize() / 2.f, tmap.size() / 2.f, 0.0f, 2.0f),
      tmap_(tmap) {
  for (auto i = 0u; i < 10u; ++i) {
    const auto entity = world_.create();
    world_.emplace<Pos>(entity, i * 1.f, i * 1.f);
    if (i % 2 == 0) {
      world_.emplace<Vel>(entity, i * 1.f, i * 1.f);
    }
  }
}

void GameState::update(GameData game) {
  auto [sm, _, window] = game;
  const auto dt = window.GetFrameTime();

  world_.view<Pos, Vel>().each([=](auto& pos, auto& vel) {
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
  camera_.target =
      rl::Vector2(camera_.target) + delta.Normalize() * 200 * dt;

  const auto tmap_size = tmap_.size();
  camera_.target = rl::Vector2(
      std::clamp(camera_.target.x, camera_.offset.x / camera_.zoom,
                 tmap_size.x - camera_.offset.x / camera_.zoom),
      std::clamp(camera_.target.y, camera_.offset.y / camera_.zoom,
                 tmap_size.y - camera_.offset.y / camera_.zoom));
}

void GameState::draw() {
  camera_.BeginMode();

  tmap_.draw(camera_);

  world_.view<Pos>().each([](const auto& pos) {
    DrawRectangle(pos.x, pos.y, 1, 1, rl::WHITE);
  });

  camera_.EndMode();
}
