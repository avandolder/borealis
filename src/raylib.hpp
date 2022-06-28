#ifndef RAYLIB_HPP
#define RAYLIB_HPP

#include <cmath>
#include <utility>

#include <stdarg.h>

namespace rl {
#include <raylib.h>
}

inline static auto operator+(rl::Vector2 u, rl::Vector2 v)
    -> rl::Vector2 {
  return {u.x + v.x, u.y + v.y};
}

inline static auto operator-(rl::Vector2 u, rl::Vector2 v)
    -> rl::Vector2 {
  return {u.x - v.x, u.y - v.y};
}

inline static auto operator*(rl::Vector2 v, float multi)
    -> rl::Vector2 {
  return {v.x * multi, v.y * multi};
}

inline static auto operator/(rl::Vector2 v, float denom)
    -> rl::Vector2 {
  return {v.x / denom, v.y / denom};
}

inline static auto operator+=(rl::Vector2& self, rl::Vector2 v)
    -> rl::Vector2& {
  self.x += v.x;
  self.y += v.y;
  return self;
}

template <class T>
inline static auto to_vector(T t) -> rl::Vector2 {
  const auto [x, y] = t;
  return {(float)x, (float)y};
}

inline static auto normalize(rl::Vector2 v) -> rl::Vector2 {
  auto length = std::sqrt(v.x * v.x + v.y * v.y);
  if (length == 0 || std::isnan(length)) return {};
  return v / length;
}

#endif /* RAYLIB_HPP */
