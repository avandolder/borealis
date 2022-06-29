#include <algorithm>

#include "game_data.hpp"
#include "game_state.hpp"
#include "pause_state.hpp"
#include "raylib.hpp"

struct Pos {
  float x;
  float y;
};

struct Vel {
  float x;
  float y;
};

GameState::GameState(TileMap& tmap)
    : tmap(tmap),
      camera{
          .offset{rl::GetScreenWidth() / 2.f,
                  rl::GetScreenHeight() / 2.f},
          .target = to_vector(tmap.size()) / 2.f,
          .rotation = 0.0f,
          .zoom = 2.0f,
      } {
  for (auto i = 0u; i < 10u; ++i) {
    const auto entity = world.create();
    world.emplace<Pos>(entity, i * 1.f, i * 1.f);
    if (i % 2 == 0) {
      world.emplace<Vel>(entity, i * 1.f, i * 1.f);
    }
  }
}

auto GameState::update(GameData& game) -> void {
  const auto dt = rl::GetFrameTime();

  world.view<Pos, Vel>().each([=](auto& pos, auto& vel) {
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
  });

  if (rl::IsKeyPressed(rl::KEY_SPACE)) {
    game.sm.push(std::make_unique<PauseState>());
  } else if (rl::IsKeyPressed(rl::KEY_ENTER)) {
    game.sm.pop();
  }

  auto dx = rl::IsKeyDown(rl::KEY_LEFT)    ? -1.f
            : rl::IsKeyDown(rl::KEY_RIGHT) ? 1.f
                                           : 0.f;
  auto dy = rl::IsKeyDown(rl::KEY_UP)     ? -1.f
            : rl::IsKeyDown(rl::KEY_DOWN) ? 1.f
                                          : 0.f;
  camera.target += normalize({dx, dy}) * 200 * dt;

  const auto tmap_size = to_vector(tmap.size());
  camera.target = {
      std::clamp(camera.target.x, camera.offset.x / camera.zoom,
                 tmap_size.x - camera.offset.x / camera.zoom),
      std::clamp(camera.target.y, camera.offset.y / camera.zoom,
                 tmap_size.y - camera.offset.y / camera.zoom),
  };
}

auto GameState::draw() -> void {
  rl::BeginMode2D(camera);

  tmap.draw(camera);

  world.view<Pos>().each([](const auto& pos) {
    rl::DrawRectangle(pos.x, pos.y, 1, 1, rl::WHITE);
  });

  rl::EndMode2D();
}
