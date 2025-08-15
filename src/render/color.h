#ifndef RENDER_COLOR_H_
#define RENDER_COLOR_H_

#include <cstdint>

namespace softy {
struct Color {
  union {
    uint32_t argb{};
    struct {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t a;
    };
  };
};
}  // namespace softy

#endif  // RENDER_COLOR_H_