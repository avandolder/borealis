#include <algorithm>  // clamp
#include <cassert>    // assert
#include <cstdlib>    // exit
#include <iostream>   // cerr, endl
#include <memory>     // make_unique, unique_ptr
#include <string>     // to_string
#include <utility>    // forward
#include <vector>     // vector

#include <tclap/CmdLine.h>
#include <entt/entt.hpp>
#include "raylib.hpp"

#include "game_data.hpp"
#include "state.hpp"
#include "state_manager.hpp"
#include "tile_map.hpp"
#include "tile_map_manager.hpp"

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
    camera.target += normalize({dx, dy}) * 200 * dt;

    const auto tmap_size = to_vector(tmap.size());
    camera.target = {
        std::clamp(camera.target.x, camera.offset.x / camera.zoom,
                   tmap_size.x - camera.offset.x / camera.zoom),
        std::clamp(camera.target.y, camera.offset.y / camera.zoom,
                   tmap_size.y - camera.offset.y / camera.zoom),
    };
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

auto parse_commands(int argc, char** argv) -> Config {
  try {
    using namespace TCLAP;
    CmdLine cmd("", ' ', "0.1", false);
    ValueArg<int> fps("f", "fps", "", false, Config::DEFAULT_FPS, "",
                      cmd);
    ValueArg<int> width("w", "width", "", false, Config::DEFAULT_WIDTH,
                        "", cmd);
    ValueArg<int> height("h", "height", "", false,
                         Config::DEFAULT_HEIGHT, "", cmd);

    cmd.parse(argc, argv);
    return {
        .width = width.getValue(),
        .height = height.getValue(),
        .fps = fps.getValue(),
    };
  } catch (TCLAP::ArgException& e) {
    std::cerr << "[ERROR] " << e.error() << " for arg " << e.argId()
              << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

auto main(int argc, char** argv) -> int {
  Config config{parse_commands(argc, argv)};

  rl::InitWindow(config.width, config.height, "borealis");
  rl::SetTargetFPS(config.fps);
  rl::SetWindowState(rl::FLAG_VSYNC_HINT);

  {
    // Create the game data within an interior scope so it can be
    // destroyed before the window is closed. Otherwise raylib
    // will segfault.
    StateManager<GameData> sm;
    TileMapManager tm;
    GameData game{sm, tm, config};

    sm.push(std::make_unique<StartState>(tm.get_map("res/island.tmx")));

    while (!rl::WindowShouldClose() && !sm.empty()) {
      sm.update(game);

      rl::BeginDrawing();
      rl::ClearBackground(rl::BLACK);

      sm.draw();

#ifdef DEBUG
      {
        const auto fps_text{std::to_string(rl::GetFPS())};
        rl::DrawText(
            fps_text.data(),
            rl::GetScreenWidth() - rl::MeasureText(fps_text.data(), 16),
            0, 16, rl::MAGENTA);
      }
#endif /* DEBUG */

      rl::EndDrawing();
    }
  }

  rl::CloseWindow();

  return 0;
}
