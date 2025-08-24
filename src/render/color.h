#ifndef RENDER_COLOR_H_
#define RENDER_COLOR_H_

#include <cstdint>

namespace softy {
struct Color {
  Color() : argb{} {}
  Color(uint32_t argb) : argb{argb} {}
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
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
}  // namespace softy

#endif  // RENDER_COLOR_H_