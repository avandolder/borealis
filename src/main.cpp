#include <cstdlib>   // exit
#include <iostream>  // cerr, endl
#include <memory>    // make_unique, unique_ptr
#include <string>    // to_string

#include <tclap/CmdLine.h>
#include <entt/entt.hpp>
#include "raylib.hpp"

#include "game_data.hpp"
#include "game_state.hpp"
#include "state_manager.hpp"
#include "tile_map_manager.hpp"

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

auto show_fps() -> void {
  const auto fps_text{std::to_string(rl::GetFPS())};
  rl::DrawText(
      fps_text.data(),
      rl::GetScreenWidth() - rl::MeasureText(fps_text.data(), 16), 0,
      16, rl::MAGENTA);
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

    sm.push(std::make_unique<GameState>(tm.get_map("res/island.tmx")));

    while (!rl::WindowShouldClose() && !sm.empty()) {
      sm.update(game);

      rl::BeginDrawing();
      rl::ClearBackground(rl::BLACK);
      sm.draw();
#ifdef DEBUG
      show_fps();
#endif
      rl::EndDrawing();
    }
  }

  rl::CloseWindow();

  return 0;
}
