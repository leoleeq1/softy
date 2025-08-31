#ifndef RENDER_MESH_H_
#define RENDER_MESH_H_

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "render/material.h"
#include "render/vertex.h"

namespace softy {
class Mesh {
 public:
  Mesh() = default;
  Mesh(const std::vector<Vertex>& vertices, const std::vector<int32_t>& indices,
       Material* material = nullptr);
  Mesh(std::vector<Vertex>&& vertices, std::vector<int32_t>& indices,
       Material* material = nullptr);
  Mesh(std::span<const Vertex> vertices, std::span<const int32_t> indices,
       Material* material = nullptr);
  Mesh(std::span<const Vertex> vertices, std::span<const std::size_t> indices,
       Material* material = nullptr);

  const std::vector<Vertex>& GetVertices() const noexcept { return vb_; }
  const std::vector<int32_t>& GetIndices() const noexcept { return ib_; }
  const Material* GetMaterial() const noexcept { return material_; }

  void SetMaterial(Material* material) noexcept { material_ = material; }

 private:
  std::vector<Vertex> vb_;
  std::vector<int32_t> ib_;
  Material* material_;
};
}  // namespace softy

#endif  // RENDER_MESH_H_