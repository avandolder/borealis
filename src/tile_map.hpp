#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <memory>
#include <string>

#include <tmx.h>
#include "raylib.hpp"

struct TileMap final {
 private:
  std::unique_ptr<tmx_map> tmap_;

  auto draw_layers(tmx_layer* layers, rl::Camera2D& camera) -> void;
  auto draw_layer(tmx_layer* layer, rl::Camera2D& camera) -> void;
  auto draw_tile(tmx_tile* tile, rl::Vector2 pos, rl::Color tint)
      -> void;
  auto draw_objects(tmx_layer* layer,
                    tmx_object_group* objgr,
                    rl::Camera2D& camera) -> void;
  auto draw_polyline(tmx_object* obj, rl::Color color) -> void;
  auto draw_polygon(tmx_object* obj, rl::Color color) -> void;

 public:
  TileMap(tmx_map*);
  TileMap(TileMap&&);
  ~TileMap();

  auto draw(rl::Camera2D& camera) -> void;

  // Size in integer pixels.
  auto size() -> std::pair<uint32_t, uint32_t>;
};

#endif /* TILE_MAP_HPP */
