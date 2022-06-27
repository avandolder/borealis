#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <memory>

struct _tmx_map;
using tmx_map = struct _tmx_map;
using tmx_resource_manager = void;

struct TileMap final {
 private:
  std::unique_ptr<tmx_map> tmap_;

 public:
  TileMap(const char* path, tmx_resource_manager* mgr);
  TileMap(TileMap&&);
  ~TileMap();
};

#endif /* TILE_MAP_HPP */
