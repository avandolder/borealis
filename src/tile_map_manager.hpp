#ifndef TILE_MAP_MANAGER_HPP
#define TILE_MAP_MANAGER_HPP

#include <string>
#include <unordered_map>

struct TileMap;

struct TileMapManager final {
 private:
  using tmx_resource_manager = void;
  tmx_resource_manager* mgr_;
  std::unordered_map<std::string, TileMap> tmaps_;

 public:
  TileMapManager();
  ~TileMapManager();

  auto get_map(const char* path) -> TileMap&;
};

#endif /* TILE_MAP_MANAGER_HPP */
