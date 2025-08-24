#ifndef RENDER_MESH_H_
#define RENDER_MESH_H_

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "render/vertex.h"

namespace softy {
class Mesh {
 public:
  Mesh() = default;
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<int32_t>& indices);
  Mesh(std::span<const Vertex> vertices, std::span<const int32_t> indices);
  Mesh(std::span<const Vertex> vertices, std::span<const std::size_t> indices);

  const std::vector<Vertex>& GetVertexBuffer() const noexcept { return vb_; }
  const std::vector<int32_t>& GetIndexBuffer() const noexcept { return ib_; }

 private:
  std::vector<Vertex> vb_;
  std::vector<int32_t> ib_;
};
}  // namespace softy

#endif  // RENDER_MESH_H_