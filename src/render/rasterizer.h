#ifndef RENDER_RASTERIZER_H_
#define RENDER_RASTERIZER_H_

#include <functional>
#include <vector>

#include "render/buffer.h"
#include "render/vertex.h"
#include "shader/shader.h"

namespace softy {
void Rasterize(ColorBuffer& renderTarget,
               const std::vector<VertexOutput>& vsOutputs,
               const std::vector<int>& indices, FragmentShader fs);
}  // namespace softy

#endif  // RENDER_RASTERIZER_H_