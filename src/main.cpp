#include <cassert>  // assert
#include <memory>   // std::make_unique, std::unique_ptr
#include <string>   // std::to_string
#include <utility>  // std::forward
#include <vector>   // std::vector

#include <entt/entt.hpp>
#include "raylib.hpp"

#include "State.hpp"
#include "StateManager.hpp"
#include "TileMapManager.hpp"

struct GameData final {
  StateManager<GameData> sm;
  TileMapManager tm;
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

  StartState() {
    for (auto i = 0u; i < 10u; ++i) {
      const auto entity = world.create();
      world.emplace<Pos>(entity, i * 1.f, i * 1.f);
      if (i % 2 == 0) {
        world.emplace<Vel>(entity, i * 1.f, i * 1.f);
      }
    }
  }

  auto update(GameData& game) -> void override {
    world.view<Pos, Vel>().each([](auto& pos, auto& vel) {
      pos.x += vel.x * rl::GetFrameTime();
      pos.y += vel.y * rl::GetFrameTime();
    });

    if (rl::IsKeyPressed(rl::KEY_SPACE)) {
      game.sm.push(std::make_unique<OverState>());
    } else if (rl::IsKeyPressed(rl::KEY_ENTER)) {
      game.sm.pop();
    }
  }

  auto draw() -> void override {
    rl::DrawText("borealis", 190, 200, 20, rl::LIGHTGRAY);

    world.view<Pos>().each([](const auto& pos) {
      rl::DrawRectangle(pos.x, pos.y, 1, 1, rl::WHITE);
    });
  }
};

auto main(void) -> int {
  rl::InitWindow(800, 450, "borealis");
  rl::SetTargetFPS(60);

  GameData game;
  game.sm.push(std::make_unique<StartState>());

  rl::Camera2D camera = {
      .offset = {0, 0},
      .target = {0, 0},
      .rotation = 0.0f,
      .zoom = 2.0f,
  };

  while (!rl::WindowShouldClose() && !game.sm.empty()) {
    game.sm.update(game);

    rl::BeginDrawing();
    rl::ClearBackground(rl::BLACK);

#ifdef DEBUG
    const auto fps_text = std::to_string(rl::GetFPS());
    const auto fps_color = [] {
      auto color = rl::GREEN;
      color.a = 127;
      return color;
    }();
    rl::DrawText(fps_text.data(), 800 - rl::MeasureText(fps_text.data(), 16), 0,
                 16, fps_color);
#endif  // DEBUG

    rl::BeginMode2D(camera);
    game.sm.draw();
    rl::EndMode2D();

    rl::EndDrawing();
  }

  rl::CloseWindow();

  return 0;
}
