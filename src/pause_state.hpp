#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include <raylib-cpp.hpp>

#include "game_data.hpp"
#include "state.hpp"

namespace rl = raylib;

struct PauseState final : public State<GameData> {
  auto update(GameData game) -> void override {
    if (IsKeyPressed(KEY_SPACE)) {
      game.sm.pop();
    }
  }

  auto draw() -> void override {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  rl::BLACK.Fade(0.5));
    rl::DrawText("over", 10, 10, 20, rl::LIGHTGRAY);
  }

  auto draw_previous() -> bool override { return true; }
};

#endif /* PAUSE_STATE_HPP */
