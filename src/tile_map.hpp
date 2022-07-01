#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <memory>
#include <string>

#include <tmx.h>
#include <raylib-cpp.hpp>

struct TileMap final {
 private:
  std::unique_ptr<tmx_map> tmap_;

  auto draw_layers(tmx_layer* layers, raylib::Camera2D& camera) -> void;
  auto draw_layer(tmx_layer* layer, raylib::Camera2D& camera) -> void;
  auto draw_tile(tmx_tile* tile,
                 raylib::Vector2 pos,
                 raylib::Color tint) -> void;
  auto draw_objects(tmx_layer* layer,
                    tmx_object_group* objgr,
                    raylib::Camera2D& camera) -> void;
  auto draw_polyline(tmx_object* obj, raylib::Color color) -> void;
  auto draw_polygon(tmx_object* obj, raylib::Color color) -> void;

 public:
  TileMap(tmx_map*);
  TileMap(TileMap&&);
  ~TileMap();

  auto draw(raylib::Camera2D& camera) -> void;

  // Size in integer pixels.
  auto size() -> raylib::Vector2;
};

#endif /* TILE_MAP_HPP */
