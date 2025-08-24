#ifndef RENDER_RENDER_PIPELINE_H_
#define RENDER_RENDER_PIPELINE_H_

#include <cstdint>
#include <vector>

#include "math/matrix.h"
#include "render/buffer.h"
#include "render/camera.h"
#include "render/material.h"
#include "render/vertex.h"

namespace softy {
class RenderPipeline {
 public:
  virtual ~RenderPipeline() = default;

  ConstantBuffer* GetConstantBuffer() { return constantBuffer_; }

  void SetConstantBuffer(ConstantBuffer* buffer) { constantBuffer_ = buffer; }

  void AddObject(const std::vector<Vertex>* vertices,
                 const std::vector<int32_t>* indices, mat4 transform,
                 const Material* material) {
    vertices_.push_back(*vertices);
    indices_.push_back(*indices);
    transforms_.push_back(transform);
    materials_.push_back(material);
  }

  virtual void Render(Camera* camera) = 0;

 protected:
  ConstantBuffer* constantBuffer_;
  std::vector<std::vector<Vertex>> vertices_;
  std::vector<std::vector<int32_t>> indices_;
  std::vector<mat4> transforms_;
  std::vector<const Material*> materials_;
};
}  // namespace softy

#endif  // RENDER_RENDER_PIPELINE_H_