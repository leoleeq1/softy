#ifndef SHADER_SHADER_H_
#define SHADER_SHADER_H_

#include <functional>
#include <vector>

#include "render/buffer.h"
#include "render/color.h"
#include "render/vertex.h"

namespace softy {
using VertexShader =
    std::function<VertexOutput(const ConstantBuffer&, const Vertex&)>;
using FragmentShader =
    std::function<Color(const ConstantBuffer&, const VertexOutput&)>;

VertexOutput DefaultVertexShader(const ConstantBuffer& cb,
                                 const Vertex& vertex);

class Shader {
 public:
  Shader(VertexShader vs, FragmentShader fs) : vs_{vs}, fs_{fs} {}
  Shader(FragmentShader fs) : vs_{DefaultVertexShader}, fs_{fs} {}

  const VertexShader GetVS() const noexcept { return vs_; }
  const FragmentShader GetFS() const noexcept { return fs_; }

 private:
  VertexShader vs_;
  FragmentShader fs_;
};

Shader UnlitColorShader();
}  // namespace softy

#endif  // SHADER_SHADER_H_