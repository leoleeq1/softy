#include "render/forward_render_pipeline.h"

#include <algorithm>
#include <ranges>
#include <utility>
#include <vector>

#include "render/buffer.h"
#include "render/camera.h"
#include "render/color.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/rasterizer.h"
#include "shader/shader.h"

namespace softy {
void ForwardRenderPipeline::Render(Camera* camera) {
  ConstantBuffer* cb = GetConstantBuffer();
  ColorBuffer* rt = camera->GetRenderTarget();

  std::vector<VertexOutput> vsOutputs;

  for (auto [mesh, transform] : std::views::zip(meshes_, transforms_)) {
    vsOutputs.clear();

    const std::vector<int32_t>& indices = mesh->GetIndices();
    const Shader* shader = mesh->GetMaterial()->GetShader();

    cb->SetWorldMatrix(transform);
    cb->SetProperties(mesh->GetMaterial()->GetProperties());
    std::ranges::transform(
        mesh->GetVertices(), std::back_inserter(vsOutputs),
        [&](const Vertex& v) { return shader->GetVS()(*cb, v); });

    Rasterize(*cb, *rt, vsOutputs, indices, shader->GetFS());
  }

  meshes_.clear();
  transforms_.clear();
}
}  // namespace softy
