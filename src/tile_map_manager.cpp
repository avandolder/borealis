#include <tmx.h>
#include <raylib-cpp.hpp>

#include "tile_map.hpp"
#include "tile_map_manager.hpp"

TileMapManager::TileMapManager() : mgr_(tmx_make_resource_manager()) {
  static auto& textures = this->textures_;  // NOLINT

  tmx_img_load_func = [](const char* path) -> void* {
    if (textures.contains(path)) return &textures[path];
    return &textures.emplace(path, path).first->second;
  };

  // Textures are freed when the manager is destructed.
  tmx_img_free_func = [](void*) -> void {};
}

TileMapManager::~TileMapManager() {
  tmx_free_resource_manager(mgr_);
}

auto TileMapManager::get_map(const char* path) -> TileMap& {
  if (tmaps_.contains(path)) return tmaps_.at(path);
  return tmaps_.emplace(path, tmx_rcmgr_load(mgr_, path)).first->second;
}
