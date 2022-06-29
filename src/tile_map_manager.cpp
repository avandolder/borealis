#include <tmx.h>
#include "raylib.hpp"

#include "tile_map.hpp"
#include "tile_map_manager.hpp"

TileMapManager::TileMapManager() : mgr_(tmx_make_resource_manager()) {
  tmx_img_load_func = [](const char* path) -> void* {
    return new auto(rl::LoadTexture(path));
  };

  tmx_img_free_func = [](void* ptr) -> void {
    auto tptr = static_cast<rl::Texture*>(ptr);
    rl::UnloadTexture(*tptr);
    delete tptr;
  };
}

TileMapManager::~TileMapManager() {
  tmx_free_resource_manager(mgr_);
}

auto TileMapManager::get_map(const char* path) -> TileMap& {
  if (tmaps_.contains(path)) return tmaps_.at(path);
  return tmaps_.emplace(path, TileMap(path, mgr_)).first->second;
}
