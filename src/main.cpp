#include <cstdlib>   // exit
#include <iostream>  // cerr, endl
#include <memory>    // make_unique, unique_ptr
#include <string>    // to_string

#include <tclap/CmdLine.h>
#include <raylib-cpp.hpp>

#include "game_data.hpp"
#include "game_state.hpp"
#include "state_manager.hpp"
#include "tile_map_manager.hpp"

namespace rl = raylib;

struct Config final {
  int width;
  int height;
  int fps;

  static const int DEFAULT_WIDTH = 1280;
  static const int DEFAULT_HEIGHT = 720;
  static const int DEFAULT_FPS = 0;
};

auto parse_commands(int argc, char** argv) -> Config {
  try {
    TCLAP::CmdLine cmd("", ' ', "0.1", false);
    TCLAP::ValueArg<int> fps("f", "fps", "", false, Config::DEFAULT_FPS,
                             "", cmd);
    TCLAP::ValueArg<int> width("w", "width", "", false,
                               Config::DEFAULT_WIDTH, "", cmd);
    TCLAP::ValueArg<int> height("h", "height", "", false,
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

auto show_fps(rl::Window& window) -> void {
  const auto fps_text{std::to_string(window.GetFPS())};
  const auto fps_text_size = 16;
  rl::DrawText(
      fps_text,
      window.GetWidth() - rl::MeasureText(fps_text, fps_text_size), 0,
      fps_text_size, rl::MAGENTA);
}

auto main(int argc, char** argv) -> int {
  rl::Window window(0, 0, "", true);
  window.SetState(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);

  Config config{parse_commands(argc, argv)};
  window.Init(config.width, config.height, "borealis");
  window.SetTargetFPS(config.fps);

  StateManager<GameData> sm;
  TileMapManager tm;
  sm.push(std::make_unique<GameState>(window,
                                      tm.get_map("res/island.tmx")));

  while (!window.ShouldClose() && !sm.empty()) {
    sm.update({sm, tm, window});

    window.BeginDrawing().ClearBackground(rl::BLACK);
    sm.draw();
#ifdef DEBUG
    show_fps(window);
#endif
    window.EndDrawing();
  }

  return 0;
}
