#ifndef RENDER_VERTEX_H_
#define RENDER_VERTEX_H_

#include "math/math.h"
#include "math/vector.h"
#include "render/color.h"

namespace softy {
struct Vertex {
  v4f position;
  v3f normal;
  v2f uv;
  Color color;
};

struct VertexOutput {
  v4f position;
  v3f normal;
  v2f uv;
  Color color;
};

constexpr Vertex lerp(Vertex v0, Vertex v1, float t) {
  Vertex v{};
  v.position = lerp(v0.position, v1.position, t);
  v.normal = lerp(v0.normal, v1.normal, t);
  v.uv = lerp(v0.uv, v1.uv, t);
  v.color = lerp(v0.color, v1.color, t);
  return v;
}

constexpr VertexOutput lerp(VertexOutput v0, VertexOutput v1, float t) {
  VertexOutput v{};
  v.position = lerp(v0.position, v1.position, t);
  v.normal = lerp(v0.normal, v1.normal, t);
  v.uv = lerp(v0.uv, v1.uv, t);
  v.color = lerp(v0.color, v1.color, t);
  return v;
}

constexpr VertexOutput lerp(const VertexOutput& v0, const VertexOutput& v1,
                            const VertexOutput& v2, v3f b) {
  VertexOutput v{};
  v.position = lerp(v0.position, v1.position, v2.position, b[0], b[1], b[2]);
  v.normal = lerp(v0.normal, v1.normal, v2.normal, b[0], b[1], b[2]);
  v.uv = lerp(v0.uv, v1.uv, v2.uv, b[0], b[1], b[2]);
  v.color = lerp(v0.color, v1.color, v2.color, b[0], b[1], b[2]);
  return v;
}
}  // namespace softy

#endif  // RENDER_VERTEX_H_