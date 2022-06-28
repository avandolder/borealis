#include <cassert>  // assert
#include <memory>   // std::make_unique, std::unique_ptr
#include <string>   // std::to_string
#include <utility>  // std::forward
#include <vector>   // std::vector

#include <entt/entt.hpp>
#include "raylib.hpp"

#include "state.hpp"
#include "state_manager.hpp"
#include "tile_map.hpp"
#include "tile_map_manager.hpp"

struct GameData final {
  StateManager<GameData> sm;
  TileMapManager tm;

  struct {
    int width = 800;
    int height = 450;
    int fps = 75;
  } config;
};

struct OverState final : public State<GameData> {
  auto update(GameData& game) -> void override {
    if (rl::IsKeyPressed(rl::KEY_SPACE)) {
      game.sm.pop();
    }
  }

  auto draw() -> void override {
    rl::DrawText("over", 10, 10, 20, rl::LIGHTGRAY);
  }

  auto draw_previous() -> bool override { return true; }
};

struct StartState final : public State<GameData> {
  entt::registry world;
  struct Pos {
    float x;
    float y;
  };
  struct Vel {
    float x;
    float y;
  };

  TileMap& tmap;
  rl::Camera2D camera;

  StartState(TileMap& tmap)
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

  auto update(GameData& game) -> void override {
    const auto dt = rl::GetFrameTime();

    world.view<Pos, Vel>().each([=](auto& pos, auto& vel) {
      pos.x += vel.x * dt;
      pos.y += vel.y * dt;
    });

    if (rl::IsKeyPressed(rl::KEY_SPACE)) {
      game.sm.push(std::make_unique<OverState>());
    } else if (rl::IsKeyPressed(rl::KEY_ENTER)) {
      game.sm.pop();
    }

    auto dx = rl::IsKeyDown(rl::KEY_LEFT)    ? -1.f
              : rl::IsKeyDown(rl::KEY_RIGHT) ? 1.f
                                             : 0.f;
    auto dy = rl::IsKeyDown(rl::KEY_UP)     ? -1.f
              : rl::IsKeyDown(rl::KEY_DOWN) ? 1.f
                                            : 0.f;
    camera.target += normalize({dx, dy}) * 100 * dt;
  }

  auto draw() -> void override {
    rl::BeginMode2D(camera);

    tmap.draw(camera);

    world.view<Pos>().each([](const auto& pos) {
      rl::DrawRectangle(pos.x, pos.y, 1, 1, rl::WHITE);
    });

    rl::EndMode2D();
  }
};

auto main(void) -> int {
  GameData game;
  rl::InitWindow(game.config.width, game.config.height, "borealis");
  rl::SetTargetFPS(rl::GetMonitorRefreshRate(rl::GetCurrentMonitor()));
  rl::SetWindowState(rl::FLAG_VSYNC_HINT);

  game.sm.push(
      std::make_unique<StartState>(game.tm.get_map("res/island.tmx")));

  while (!rl::WindowShouldClose() && !game.sm.empty()) {
    game.sm.update(game);

    rl::BeginDrawing();
    rl::ClearBackground(rl::BLACK);

    game.sm.draw();

#ifdef DEBUG
    {
      const auto fps_text{std::to_string(rl::GetFPS())};
      rl::DrawText(
          fps_text.data(),
          game.config.width - rl::MeasureText(fps_text.data(), 16), 0,
          16, rl::MAGENTA);
    }
#endif /* DEBUG */

    rl::EndDrawing();
  }

  rl::CloseWindow();

  return 0;
}
