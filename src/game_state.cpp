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

struct Shape final {
  int w = 0;
  int h = 0;
  rl::Color color;
};

struct Player final {
  rl::Vector2 offset;
  static constexpr const float max_offset = 50;
};

static auto clamp(const rl::Vector2& v,
                  const rl::Vector2& lo,
                  const rl::Vector2& hi) -> rl::Vector2 {
  return {std::clamp(v.x, lo.x, hi.x), std::clamp(v.y, lo.y, hi.y)};
}

GameState::GameState(rl::Window& window, TileMap& tmap)
    : camera_(window.GetSize() / 2.f, rl::Vector2::Zero(), 0.0f, 3.f),
      tmap_(tmap),
      player_(world_.create()) {
  world_.emplace<Pos>(player_, tmap.size() / 2.f);
  camera_.target = tmap.size() / 2.f;

  world_.emplace<Vel>(player_, rl::Vector2::Zero());
  world_.emplace<Shape>(player_,
                        Shape{.w = 10, .h = 10, .color = rl::BLACK});
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
  auto& vel = world_.get<Vel>(player_);
  vel = Vel(delta * 200 * dt);

  const auto tmap_size = tmap_.size();
  auto& pos = world_.get<Pos>(player_);
  pos = Pos(clamp(rl::Vector2::Zero(), pos + vel, tmap_size));

  auto& [offset] = world_.get<Player>(player_);
  if (delta == rl::Vector2::Zero()) {
    // When not moving, tween the camera towards the player.
    auto progress = offset.Length() / Player::max_offset;
    offset = offset.LengthSqr() < 1
                 ? rl::Vector2{0, 0}
                 : rl::Vector2{offset * (1 - progress * dt * 5)};
  } else {
    auto old_offset{offset};
    offset -= vel;
    if (offset.Length() > Player::max_offset) {
      offset = old_offset;
    }
  }

  camera_.target =
      clamp(pos + offset, rl::Vector2{camera_.offset} / camera_.zoom,
            tmap_size - rl::Vector2{camera_.offset} / camera_.zoom);
}

void GameState::draw() {
  camera_.BeginMode();

  tmap_.draw(camera_);

  world_.view<Pos, Shape>().each(
      [](const auto& pos, const auto& shape) {
        DrawRectangle(pos.x - shape.w / 2, pos.y - shape.h / 2, shape.w,
                      shape.h, shape.color);
      });

#ifdef DEBUG
  DrawCircleLines(camera_.target.x, camera_.target.y,
                  Player::max_offset, rl::Color::Magenta());
#endif

  camera_.EndMode();
}
