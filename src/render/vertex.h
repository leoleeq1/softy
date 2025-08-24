#ifndef RENDER_VERTEX_H_
#define RENDER_VERTEX_H_

#include "math/math.h"
#include "math/vector.h"

namespace softy {
struct Vertex {
  v4f position;
  v3f normal;
  v2f uv;
};

struct VertexOutput {
  v4f position;
  v3f normal;
  v2f uv;
};

constexpr Vertex lerp(Vertex v0, Vertex v1, float t) {
  Vertex v{};
  v.position = lerp(v0.position, v1.position, t);
  v.normal = lerp(v0.normal, v1.normal, t);
  v.uv = lerp(v0.uv, v1.uv, t);
  return v;
}

constexpr VertexOutput lerp(VertexOutput v0, VertexOutput v1, float t) {
  VertexOutput v{};
  v.position = lerp(v0.position, v1.position, t);
  v.normal = lerp(v0.normal, v1.normal, t);
  v.uv = lerp(v0.uv, v1.uv, t);
  return v;
}
}  // namespace softy

#endif  // RENDER_VERTEX_H_