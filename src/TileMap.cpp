#include "raylib.hpp"
#include "tmx.hpp"

#include "TileMap.hpp"

static inline auto int_to_color(int color) -> rl::Color {
  auto res = tmx_col_to_bytes(color);
  return *reinterpret_cast<rl::Color*>(&res);
}

TileMap::TileMap(const char* path, tmx_resource_manager* mgr)
    : tmap_(tmx_rcmgr_load(mgr, path)) {}

TileMap::TileMap(TileMap&& other) {
  tmap_.swap(other.tmap_);
}

TileMap::~TileMap() {
  tmx_map_free(tmap_.release());
}
