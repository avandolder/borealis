#include <cassert>  // assert
#include <memory>   // std::make_unique, std::unique_ptr
#include <string>   // std::to_string
#include <utility>  // std::forward
#include <vector>   // std::vector

#include <entt/entt.hpp>

namespace rl {
#include <raylib.h>
}

struct StateManager;

struct State {
  virtual ~State() {}
  virtual auto update(StateManager& sm) -> void = 0;
  virtual auto draw() -> void = 0;
  virtual inline auto draw_previous() const -> bool { return false; }
};

using StatePtr = std::unique_ptr<State>;

struct StateManager {
 private:
  std::vector<StatePtr> states_;

 public:
  auto push(StatePtr&& st) -> void { states_.push_back(std::move(st)); }

  auto pop() -> void { states_.pop_back(); }

  auto replace(StatePtr&& st) {
    pop();
    push(std::forward<StatePtr>(st));
  }

  auto update() -> void {
    assert(!states_.empty());
    states_.back()->update(*this);
  }

  auto draw() -> void {
    assert(!states_.empty());
    auto iter = --states_.cend();
    for (; iter != states_.cbegin() && (*iter)->draw_previous(); --iter) {
    }
    do {
      (*iter)->draw();
    } while (++iter != states_.cend());
  }

  inline auto empty() const -> bool { return states_.empty(); }
};

struct OverState : public State {
  auto update(StateManager& sm) -> void override {
    if (rl::IsKeyPressed(rl::KEY_SPACE)) {
      sm.pop();
    }
  }
  auto draw() -> void override {
    rl::DrawText("over", 10, 10, 20, rl::LIGHTGRAY);
  }
  inline auto draw_previous() const -> bool override { return true; }
};

struct StartState : public State {
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
      pos.x += vel.x * rl::GetFrameTime();
      pos.y += vel.y * rl::GetFrameTime();
    });

    if (rl::IsKeyPressed(rl::KEY_SPACE)) {
      sm.push(std::make_unique<OverState>());
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
  StateManager sm;
  sm.push(std::make_unique<StartState>());

  rl::InitWindow(800, 450, "borealis");
  // rl::SetTargetFPS(60);

  while (!rl::WindowShouldClose() && !sm.empty()) {
    sm.update();

    rl::BeginDrawing();
    rl::ClearBackground(rl::BLACK);
#ifdef DEBUG
    const auto fps_text = std::to_string(rl::GetFPS());
    auto fps_color = rl::GREEN;
    fps_color.a = 127;
    rl::DrawText(fps_text.data(), 800 - rl::MeasureText(fps_text.data(), 20), 0,
                 20, fps_color);
#endif  // DEBUG
    sm.draw();
    rl::EndDrawing();
  }

  rl::CloseWindow();

  return 0;
}
