#include <cassert> // assert
#include <memory>  // std::make_unique, std::unique_ptr
#include <sstream> // std::stringstream
#include <utility> // std::forward
#include <vector>  // std::vector

#include <entt/entt.hpp>
#include <raylib.h>

struct StateManager;

struct State {
  virtual ~State() {}
  virtual auto update(StateManager &sm) -> void = 0;
  virtual auto draw() const -> void = 0;
  virtual inline auto draw_previous() const -> bool { return false; }
};

using StatePtr = std::unique_ptr<State>;

struct StateManager {
private:
  std::vector<StatePtr> states_;

public:
  auto push(StatePtr &&st) -> void { states_.push_back(std::move(st)); }

  auto pop() -> void { states_.pop_back(); }

  auto replace(StatePtr &&st) {
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

struct StartState : public State {
  entt::registry registry;
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
      const auto entity = registry.create();
      registry.emplace<Pos>(entity, i * 1.f, i * 1.f);
      if (i % 2 == 0) {
        registry.emplace<Vel>(entity, i * .1f, i * .1f);
      }
    }
  }

  auto update(StateManager &sm) -> void override {
    registry.view<Pos, Vel>().each([](auto &pos, auto &vel) {
      pos.x += vel.x;
      pos.y += vel.y;
    });
  }

  auto draw() const -> void override {
    DrawText("borealis", 190, 200, 20, LIGHTGRAY);

    int i = 0;
    registry.view<Pos>().each([&i](const auto &pos) {
      std::stringstream stream;
      stream << pos.x << " " << pos.y;
      auto as_str = stream.str();
      DrawText(as_str.c_str(), 10, 10 * ++i, 10, WHITE);
    });
  }
};

struct MidState : public State {
  auto update(StateManager &sm) -> void override {}
  auto draw() const -> void override {
    DrawText("mid", 200, 10, 20, LIGHTGRAY);
  }
};

struct OverState : public State {
  auto update(StateManager &sm) -> void override {}
  auto draw() const -> void override {
    DrawText("over", 10, 10, 20, LIGHTGRAY);
  }
  inline auto draw_previous() const -> bool override { return true; }
};

auto main(void) -> int {
  StateManager sm;
  sm.push(std::make_unique<StartState>());
  // sm.push(std::make_unique<MidState>());
  // sm.push(std::make_unique<OverState>());

  InitWindow(800, 450, "borealis");

  while (!WindowShouldClose() && !sm.empty()) {
    sm.update();

    BeginDrawing();
    ClearBackground(BLACK);
    sm.draw();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
