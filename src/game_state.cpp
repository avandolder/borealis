#include <algorithm>

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

#include "game_data.hpp"
#include "game_state.hpp"
#include "pause_state.hpp"

namespace rl = raylib;

struct Pos final : rl::Vector2 {
  explicit Pos(rl::Vector2 v) : rl::Vector2(v) {}
  Pos(float x, float y) : rl::Vector2(x, y) {}
};
struct Vel final : rl::Vector2 {
  explicit Vel(rl::Vector2 v) : rl::Vector2(v) {}
  Vel(float x, float y) : rl::Vector2(x, y) {}
};

struct Player final {};

GameState::GameState(rl::Window& window, TileMap& tmap)
    : camera_(window.GetSize() / 2.f, rl::Vector2::Zero(), 0.0f, 2.f),
      tmap_(tmap),
      player_(world_.create()) {
  world_.emplace<Pos>(player_, tmap.size() / 2.f);
  world_.emplace<Vel>(player_, rl::Vector2::Zero());
  world_.emplace<Player>(player_);
}

void GameState::update(GameData game) {
  auto [sm, _, window] = game;
  const auto dt = window.GetFrameTime();

  if (IsKeyPressed(KEY_SPACE)) {
    sm.push(std::make_unique<PauseState>());
  } else if (IsKeyPressed(KEY_ENTER)) {
    sm.pop();
  }

  const auto delta =
      rl::Vector2{IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)    ? -1.f
                  : IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) ? 1.f
                                                             : 0.f,
                  IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)     ? -1.f
                  : IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) ? 1.f
                                                            : 0.f}
          .Normalize();
  world_.get<Vel>(player_) = Vel(delta * 200);

  const auto tmap_size = tmap_.size();
  world_.view<Pos, Vel>().each([&](auto& pos, auto& vel) {
    pos.x = std::clamp(0.f, pos.x + vel.x * dt, tmap_size.x);
    pos.y = std::clamp(0.f, pos.y + vel.y * dt, tmap_size.y);
  });

  const auto& pos = world_.get<Pos>(player_);
  camera_.target = rl::Vector2(
      std::clamp(pos.x, camera_.offset.x / camera_.zoom,
                 tmap_size.x - camera_.offset.x / camera_.zoom),
      std::clamp(pos.y, camera_.offset.y / camera_.zoom,
                 tmap_size.y - camera_.offset.y / camera_.zoom));
}

void GameState::draw() {
  camera_.BeginMode();

  tmap_.draw(camera_);

  world_.view<Pos>().each([](const auto& pos) {
    DrawRectangle(pos.x, pos.y, 10, 10, rl::WHITE);
  });

  camera_.EndMode();
}
