#include <tmx.h>
#include "raylib.hpp"

#include "TileMap.hpp"
#include "TileMapManager.hpp"

TileMapManager::TileMapManager() : mgr_(tmx_make_resource_manager()) {
  tmx_img_load_func = [](const char* path) -> void* {
    auto t = new auto(rl::LoadTexture(path));
    return t;
  };

  tmx_img_free_func = [](void* ptr) -> void {
    auto tptr = static_cast<rl::Texture*>(ptr);
    rl::UnloadTexture(*tptr);
    delete tptr;
  };
}

TileMapManager::~TileMapManager() {
  tmaps_.clear();
  //tmx_free_resource_manager(mgr_);
}

auto TileMapManager::get_map(const char* path) -> TileMap& {
  if (tmaps_.contains(path))
    return tmaps_.at(path);
  return tmaps_.emplace(path, TileMap(path, mgr_)).first->second;
}
