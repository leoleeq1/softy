#include "render/mesh.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <vector>

#include "mesh.h"
#include "render/vertex.h"

namespace softy {
Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<int32_t>& indices)
    : vb_{vertices}, ib_{indices} {}

Mesh::Mesh(std::span<const Vertex> vertices, std::span<const int32_t> indices) {
  vb_.resize(vertices.size());
  ib_.resize(indices.size());
  std::ranges::copy(vertices, vb_.begin());
  std::ranges::copy(indices, ib_.begin());
}

Mesh::Mesh(std::span<const Vertex> vertices,
           std::span<const std::size_t> indices) {
  vb_.resize(vertices.size());
  ib_.resize(indices.size());
  std::ranges::copy(vertices, vb_.begin());
  std::ranges::copy(indices | std::views::transform([](std::size_t i) {
                      return static_cast<int32_t>(i);
                    }),
                    ib_.begin());
}
}  // namespace softy
