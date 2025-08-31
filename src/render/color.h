#ifndef RENDER_COLOR_H_
#define RENDER_COLOR_H_

#include <cstdint>

#include "math/math.h"

namespace softy {
struct Color {
  constexpr Color() : argb{} {}
  constexpr Color(uint32_t argb) : argb{argb} {}
  constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
      : b{b}, g{g}, r{r}, a{a} {}

  union {
    uint32_t argb{};
    struct {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t a;
    };
  };

  static Color White() { return Color{0xFFFFFFFF}; };
  static Color Black() { return Color{0xFF000000}; };
  static Color Red() { return Color{0xFFFF0000}; };
  static Color Green() { return Color{0xFF00FF00}; };
  static Color Blue() { return Color{0xFF0000FF}; };
};

constexpr uint8_t ClampColor(int32_t color) {
  if (color > 0xFF) {
    return static_cast<uint8_t>(0xFF);
  }
  if (color < 0x00) {
    return static_cast<uint8_t>(0x00);
  }
  return static_cast<uint8_t>(color);
}

constexpr uint8_t ClampColor(float color) {
  if (color > 1.0f) {
    return static_cast<uint8_t>(0xFF);
  }
  if (color < 0.0f) {
    return static_cast<uint8_t>(0x00);
  }
  return static_cast<uint8_t>(
      clamp(static_cast<int32_t>(color * 0xFF), 0, 0xFF));
}

constexpr Color operator+(Color lhs, Color rhs) {
  Color color{};
  color.r = ClampColor(lhs.r + rhs.r);
  color.g = ClampColor(lhs.g + rhs.g);
  color.b = ClampColor(lhs.b + rhs.b);
  color.a = ClampColor(lhs.a + rhs.a);
  return color;
}

constexpr Color operator-(Color lhs, Color rhs) {
  Color color{};
  color.r = ClampColor(lhs.r - rhs.r);
  color.g = ClampColor(lhs.g - rhs.g);
  color.b = ClampColor(lhs.b - rhs.b);
  color.a = ClampColor(lhs.a - rhs.a);
  return color;
}

constexpr Color operator*(Color lhs, Color rhs) {
  Color color{};
  color.r = ClampColor(static_cast<float>(lhs.r) / 0xFF *
                       static_cast<float>(rhs.r) / 0xFF);
  color.g = ClampColor(static_cast<float>(lhs.g) / 0xFF *
                       static_cast<float>(rhs.g) / 0xFF);
  color.b = ClampColor(static_cast<float>(lhs.b) / 0xFF *
                       static_cast<float>(rhs.b) / 0xFF);
  color.a = ClampColor(static_cast<float>(lhs.a) / 0xFF *
                       static_cast<float>(rhs.a) / 0xFF);
  return color;
}

constexpr Color operator*(float lhs, Color rhs) {
  Color color{};
  color.r = ClampColor(lhs * static_cast<float>(rhs.r) / 0xFF);
  color.g = ClampColor(lhs * static_cast<float>(rhs.g) / 0xFF);
  color.b = ClampColor(lhs * static_cast<float>(rhs.b) / 0xFF);
  color.a = ClampColor(lhs * static_cast<float>(rhs.a) / 0xFF);
  return color;
}

constexpr Color operator*(Color lhs, float rhs) {
  Color color{};
  color.r = ClampColor(static_cast<float>(lhs.r) / 0xFF * rhs);
  color.g = ClampColor(static_cast<float>(lhs.g) / 0xFF * rhs);
  color.b = ClampColor(static_cast<float>(lhs.b) / 0xFF * rhs);
  color.a = ClampColor(static_cast<float>(lhs.a) / 0xFF * rhs);
  return color;
}
}  // namespace softy

#endif  // RENDER_COLOR_H_