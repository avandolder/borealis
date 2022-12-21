#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <memory>
#include <string>

#include <tmx.h>
#include <raylib-cpp.hpp>

struct TileMap final {
 private:
  std::unique_ptr<tmx_map> tmap_;

  void draw_layers(tmx_layer* layers, raylib::Camera2D& camera);
  void draw_layer(tmx_layer* layer, raylib::Camera2D& camera);
  void draw_tile(tmx_tile* tile,
                 raylib::Vector2 pos,
                 raylib::Color tint);
  void draw_objects(tmx_layer* layer,
                    tmx_object_group* objgr,
                    raylib::Camera2D& camera);
  void draw_polyline(tmx_object* obj, raylib::Color color);
  void draw_polygon(tmx_object* obj, raylib::Color color);

 public:
  explicit TileMap(tmx_map*);
  TileMap(TileMap&&) noexcept;
  ~TileMap();

  TileMap(TileMap&) = delete;
  void operator=(TileMap&) = delete;
  void operator=(TileMap&&) = delete;

  void draw(raylib::Camera2D& camera);

  // Size in integer pixels.
  auto size() -> raylib::Vector2;
};

#endif /* TILE_MAP_HPP */
