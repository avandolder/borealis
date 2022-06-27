#include <cassert>  // assert
#include <memory>   // std::make_unique, std::unique_ptr
#include <string>   // std::to_string
#include <utility>  // std::forward
#include <vector>   // std::vector

#include <raylib.h>
#include <entt/entt.hpp>

#include "State.hpp"
#include "StateManager.hpp"
#include "TileMapManager.hpp"

struct OverState final : public State {
  auto update(StateManager& sm) -> void override {
    if (IsKeyPressed(KEY_SPACE)) {
      sm.pop();
    }
  }
  auto draw() -> void override { DrawText("over", 10, 10, 20, LIGHTGRAY); }
  auto draw_previous() -> bool override { return true; }
};

struct StartState final : public State {
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

  auto update(StateManager& sm) -> void override {
    world.view<Pos, Vel>().each([](auto& pos, auto& vel) {
      pos.x += vel.x * GetFrameTime();
      pos.y += vel.y * GetFrameTime();
    });

    if (IsKeyPressed(KEY_SPACE)) {
      sm.push(std::make_unique<OverState>());
    } else if (IsKeyPressed(KEY_ENTER)) {
      sm.pop();
    }
  }

  auto draw() -> void override {
    DrawText("borealis", 190, 200, 20, LIGHTGRAY);

    world.view<Pos>().each(
        [](const auto& pos) { DrawRectangle(pos.x, pos.y, 1, 1, WHITE); });
  }
};

auto main(void) -> int {
  InitWindow(800, 450, "borealis");
  SetTargetFPS(60);

  StateManager sm;
  sm.push(std::make_unique<StartState>());

  TileMapManager tm;

  Camera2D camera = {
      .offset = {0, 0},
      .target = {0, 0},
      .rotation = 0.0f,
      .zoom = 2.0f,
  };

  while (!WindowShouldClose() && !sm.empty()) {
    sm.update();

    BeginDrawing();
    ClearBackground(BLACK);

#ifdef DEBUG
    const auto fps_text = std::to_string(GetFPS());
    const auto fps_color = []() {
      auto color = GREEN;
      color.a = 127;
      return color;
    }();
    DrawText(fps_text.data(), 800 - MeasureText(fps_text.data(), 16), 0, 16,
             fps_color);
#endif  // DEBUG

    BeginMode2D(camera);
    sm.draw();
    EndMode2D();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
