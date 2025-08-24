#include "render/forward_render_pipeline.h"

#include <vector>

#include "forward_render_pipeline.h"
#include "render/buffer.h"
#include "render/camera.h"
#include "render/color.h"
#include "render/material.h"
#include "render/rasterizer.h"
#include "shader/shader.h"

namespace softy {
void ForwardRenderPipeline::Render(Camera* camera) {
  ConstantBuffer* cb = GetConstantBuffer();
  ColorBuffer* rt = camera->GetRenderTarget();

  for (std::size_t i = 0; i < vertices_.size(); ++i) {
    std::vector<Vertex>& vertices = vertices_[i];
    std::vector<int32_t>& indices = indices_[i];
    mat4 transform = transforms_[i];
    const Material* material = materials_[i];

    cb->SetWorldMatrix(transform);
    std::vector<VertexOutput> outputs =
        material->GetShader()->GetVS()(*cb, vertices);

    Rasterize(*rt, outputs, indices, material->GetShader()->GetFS());
  }

  vertices_.clear();
  indices_.clear();
  transforms_.clear();
  materials_.clear();
}
}  // namespace softy
