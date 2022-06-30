#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include "game_data.hpp"
#include "raylib.hpp"
#include "state.hpp"

struct PauseState final : public State<GameData> {
  auto update(GameData& game) -> void override {
    if (rl::IsKeyPressed(rl::KEY_SPACE)) {
      game.sm.pop();
    }
  }

  auto draw() -> void override {
    rl::DrawRectangle(0, 0, rl::GetScreenWidth(), rl::GetScreenHeight(),
                      rl::Fade(rl::BLACK, 0.5));
    rl::DrawText("over", 10, 10, 20, rl::LIGHTGRAY);
  }

  auto draw_previous() -> bool override { return true; }
};

#endif /* PAUSE_STATE_HPP */
