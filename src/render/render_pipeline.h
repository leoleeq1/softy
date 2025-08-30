#ifndef RENDER_RENDER_PIPELINE_H_
#define RENDER_RENDER_PIPELINE_H_

#include <cstdint>
#include <vector>

#include "math/matrix.h"
#include "render/buffer.h"
#include "render/camera.h"
#include "render/material.h"
#include "render/mesh.h"

namespace softy {
class RenderPipeline {
 public:
  virtual ~RenderPipeline() = default;

  ConstantBuffer* GetConstantBuffer() { return constantBuffer_; }

  void SetConstantBuffer(ConstantBuffer* buffer) { constantBuffer_ = buffer; }

  void AddObject(const Mesh* mesh, mat4 transform) {
    meshes_.push_back(mesh);
    transforms_.push_back(transform);
  }

  virtual void Render(Camera* camera) = 0;

 protected:
  ConstantBuffer* constantBuffer_;
  std::vector<const Mesh*> meshes_;
  std::vector<mat4> transforms_;
};
}  // namespace softy

#endif  // RENDER_RENDER_PIPELINE_H_