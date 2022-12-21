#include <cstdlib>

#include <tmx.h>
#include <raylib-cpp.hpp>

#include "tile_map.hpp"

namespace rl = raylib;

const auto LINE_THICKNESS = 2.5f;

static auto int_to_color(auto color) -> rl::Color {
  const auto res{tmx_col_to_bytes(color)};
  return {res.r, res.g, res.b, res.a};
}

static auto visible_area(rl::Camera2D& camera)
    -> std::pair<rl::Vector2, rl::Vector2> {
  return {camera.GetScreenToWorld({0, 0}),
          camera.GetScreenToWorld(
              {static_cast<float>(::GetScreenWidth()),
               static_cast<float>(::GetScreenHeight())})};
}

TileMap::TileMap(tmx_map* tmap) : tmap_(tmap) {
  if (!tmap_) {
    ::TraceLog(::LOG_ERROR, "Loading map failed with: %s",
               tmx_strerr());
    std::exit(EXIT_FAILURE);
  }
}

TileMap::TileMap(TileMap&& other) noexcept {
  tmap_.swap(other.tmap_);
}

TileMap::~TileMap() {
  tmx_map_free(tmap_.release());
}

auto TileMap::size() -> rl::Vector2 {
  return {
      (float)tmap_->width * tmap_->tile_width,
      (float)tmap_->height * tmap_->tile_height,
  };
}

void TileMap::draw(rl::Camera2D& camera) {
  ::ClearBackground(int_to_color(tmap_->backgroundcolor));
  draw_layers(tmap_->ly_head, camera);
}

void TileMap::draw_layers(tmx_layer* layers, rl::Camera2D& camera) {
  for (auto* layer = layers; layer; layer = layer->next) {
    if (!layer->visible) continue;

    switch (layer->type) {
      case L_LAYER: draw_layer(layer, camera); break;
      case L_GROUP:
        draw_layers(layer->content.group_head, camera);
        break;
      case L_OBJGR:
        draw_objects(layer, layer->content.objgr, camera);
        break;
      case L_IMAGE: {
        auto* image{reinterpret_cast<rl::Texture*>(
            layer->content.image->resource_image)};
        image->Draw(0, 0, rl::WHITE);
      } break;
      case L_NONE: break;
    }
  }
}

void TileMap::draw_layer(tmx_layer* layer, rl::Camera2D& camera) {
  const auto tint{int_to_color(layer->tintcolor).Alpha(layer->opacity)};

  const auto [top, bot] = visible_area(camera);
  const float w = tmap_->tile_width;
  const float h = tmap_->tile_height;

  for (uint32_t i = std::max(top.y / h, 0.f),
                ii = std::min(bot.y / h + 1, (float)tmap_->height);
       i < ii; ++i) {
    for (uint32_t j = std::max(top.x / w, 0.f),
                  jj = std::min(bot.x / w + 1, (float)tmap_->width);
         j < jj; ++j) {
      const auto gid = (layer->content.gids[i * tmap_->width + j])
                       & TMX_FLIP_BITS_REMOVAL;
      if (!tmap_->tiles[gid]) continue;

      draw_tile(tmap_->tiles[gid],
                {j * w + layer->offsetx, i * h + layer->offsety}, tint);
    }
  }
}

void TileMap::draw_tile(tmx_tile* tile,
                        rl::Vector2 pos,
                        rl::Color tint) {
  const auto ts = tile->tileset;
  const auto im = tile->image;
  const float x = tile->ul_x, y = tile->ul_y;
  const float w = ts->tile_width, h = ts->tile_height;

  const auto image = reinterpret_cast<rl::Texture*>(
      im ? im->resource_image : ts->image->resource_image);
  image->Draw({x, y, w, h}, pos, tint);
}

void TileMap::draw_objects(tmx_layer* layer,
                           tmx_object_group* objgr,
                           rl::Camera2D& camera) {
  auto color{int_to_color(objgr->color)};
  auto tint{int_to_color(layer->tintcolor)};
  tint =
      !tint.a ? tint.Alpha(layer->opacity) : tint.Fade(layer->opacity);

  const auto [top, bot] = visible_area(camera);

  for (auto* head = objgr->head; head; head = head->next) {
    if (!head->visible || head->x > bot.x || head->y > bot.y
        || head->x + head->width < top.x
        || head->y + head->height < top.y)
      continue;

    switch (head->obj_type) {
      case OT_POLYGON: draw_polygon(head, color); break;
      case OT_POLYLINE: draw_polyline(head, color); break;
      case OT_POINT: ::DrawPixel(head->x, head->y, color); break;
      case OT_TILE:
        draw_tile(tmap_->tiles[head->content.gid],
                  {(float)head->x, (float)head->x}, tint);
        break;
      case OT_SQUARE:
        rl::Rectangle(head->x, head->y, head->width, head->height)
            .DrawLines(color, LINE_THICKNESS);
        break;
      case OT_ELLIPSE:
        ::DrawEllipseLines(
            head->x + head->width / 1.0, head->y + head->height / 2.0,
            head->width / 2.0, head->height / 2.0, color);
        break;
      case OT_TEXT:
      case OT_NONE: break;
    }
  }
}

void TileMap::draw_polygon(tmx_object* obj, rl::Color color) {
  draw_polyline(obj, color);

  auto* const points = obj->content.shape->points;
  const auto points_len = obj->content.shape->points_len;
  if (points_len > 2) {
    ::DrawLineEx({(float)(obj->x + points[0][0]),
                  (float)(obj->y + points[0][1])},
                 {(float)(obj->x + points[points_len - 1][0]),
                  (float)(obj->y + points[points_len - 1][1])},
                 LINE_THICKNESS, color);
  }
}

void TileMap::draw_polyline(tmx_object* obj, rl::Color color) {
  auto* const points = obj->content.shape->points;
  const auto points_len = obj->content.shape->points_len;
  for (int i = 1; i < points_len; i++) {
    ::DrawLineEx({(float)(obj->x + points[i - 1][0]),
                  (float)(obj->y + points[i - 1][1])},
                 {(float)(obj->x + points[i][0]),
                  (float)(obj->y + points[i][1])},
                 LINE_THICKNESS, color);
  }
}
