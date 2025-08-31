#include "geometry/generator.h"

#include <array>
#include <memory>

#include "math/math.h"
#include "math/vector.h"
#include "render/color.h"
#include "render/mesh.h"
#include "render/vertex.h"

namespace softy {
std::unique_ptr<Mesh> CreateCubeVertex8() {
  std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(Mesh{
      std::array{
          Vertex(v4f{-0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{-1.0f, -1.0f, -1.0f}), v2f{}, Color{0xFFFFFFFF}),
          Vertex(v4f{-0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{-1.0f, -1.0f, +1.0f}), v2f{}, Color{0xFFFFFFFF}),
          Vertex(v4f{+0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{+1.0f, -1.0f, +1.0f}), v2f{}, Color{0xFFFFFFFF}),
          Vertex(v4f{+0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{+1.0f, -1.0f, -1.0f}), v2f{}, Color{0xFFFFFFFF}),
          Vertex(v4f{-0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{-1.0f, +1.0f, -1.0f}), v2f{}, Color{0xFFFFFFFF}),
          Vertex(v4f{-0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{-1.0f, +1.0f, +1.0f}), v2f{}, Color{0xFFFFFFFF}),
          Vertex(v4f{+0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{+1.0f, +1.0f, -1.0f}), v2f{}, Color{0xFFFFFFFF}),
          Vertex(v4f{+0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{+1.0f, +1.0f, +1.0f}), v2f{}, Color{0xFFFFFFFF}),
      },
      std::array<std::size_t, 36>{0, 2, 1, 0, 3, 2, 0, 4, 7, 0, 7, 3,
                                  1, 5, 4, 1, 4, 0, 2, 6, 5, 2, 5, 1,
                                  3, 7, 6, 3, 6, 2, 4, 5, 6, 4, 6, 7},
  });

  return mesh;
}

std::unique_ptr<Mesh> softy::CreateCube() {
  std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(Mesh{
      std::array{
          // Bottom
          Vertex(v4f{-0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, -1.0f, 0.0f}), v2f{0.0f, 0.0f},
                 Color{0xFF000000}),
          Vertex(v4f{-0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, -1.0f, 0.0f}), v2f{0.0f, 1.0f},
                 Color{0xFFFF0000}),
          Vertex(v4f{+0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, -1.0f, 0.0f}), v2f{1.0f, 1.0f},
                 Color{0xFF00FF00}),
          Vertex(v4f{+0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, -1.0f, 0.0f}), v2f{1.0f, 0.0f},
                 Color{0xFF0000FF}),
          // Top
          Vertex(v4f{-0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{0.0f, 0.0f},
                 Color{0xFF000000}),
          Vertex(v4f{-0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{0.0f, 1.0f},
                 Color{0xFFFF0000}),
          Vertex(v4f{+0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{1.0f, 1.0f},
                 Color{0xFF00FF00}),
          Vertex(v4f{+0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{1.0f, 0.0f},
                 Color{0xFF0000FF}),
          // Left
          Vertex(v4f{-0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{-1.0f, 0.0f, 0.0f}), v2f{0.0f, 0.0f},
                 Color{0xFF000000}),
          Vertex(v4f{-0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{-1.0f, 0.0f, 0.0f}), v2f{0.0f, 1.0f},
                 Color{0xFFFF0000}),
          Vertex(v4f{-0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{-1.0f, 0.0f, 0.0f}), v2f{1.0f, 1.0f},
                 Color{0xFF00FF00}),
          Vertex(v4f{-0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{-1.0f, 0.0f, 0.0f}), v2f{1.0f, 0.0f},
                 Color{0xFF0000FF}),
          // Right
          Vertex(v4f{+0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{0.0f, 0.0f},
                 Color{0xFF000000}),
          Vertex(v4f{+0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{0.0f, 1.0f},
                 Color{0xFFFF0000}),
          Vertex(v4f{+0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{1.0f, 1.0f},
                 Color{0xFF00FF00}),
          Vertex(v4f{+0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, +1.0f, 0.0f}), v2f{1.0f, 0.0f},
                 Color{0xFF0000FF}),
          // Back
          Vertex(v4f{-0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, -1.0f}), v2f{0.0f, 0.0f},
                 Color{0xFF000000}),
          Vertex(v4f{-0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, -1.0f}), v2f{0.0f, 1.0f},
                 Color{0xFFFF0000}),
          Vertex(v4f{+0.5f, +0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, -1.0f}), v2f{1.0f, 1.0f},
                 Color{0xFF00FF00}),
          Vertex(v4f{+0.5f, -0.5f, -0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, -1.0f}), v2f{1.0f, 0.0f},
                 Color{0xFF0000FF}),
          // Front
          Vertex(v4f{-0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, +1.0f}), v2f{0.0f, 0.0f},
                 Color{0xFF000000}),
          Vertex(v4f{-0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, +1.0f}), v2f{0.0f, 1.0f},
                 Color{0xFFFF0000}),
          Vertex(v4f{+0.5f, +0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, +1.0f}), v2f{1.0f, 1.0f},
                 Color{0xFF00FF00}),
          Vertex(v4f{+0.5f, -0.5f, +0.5f, 1.0f},
                 normalize(v3f{0.0f, 0.0f, +1.0f}), v2f{1.0f, 0.0f},
                 Color{0xFF0000FF}),

      },
      std::array<std::size_t, 36>{
          0,  2,  1,  0,  3,  2,  16, 17, 18, 16, 18, 19, 9, 10, 11, 9, 11, 8,
          23, 22, 21, 23, 21, 20, 12, 15, 14, 12, 14, 13, 4, 5,  6,  4, 6,  7},
  });

  return mesh;
}
}  // namespace softy