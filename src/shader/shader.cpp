#include "shader/shader.h"

#include <any>

namespace softy {
std::vector<VertexOutput> softy::DefaultVertexShader(
    const ConstantBuffer& cb, const std::vector<Vertex>& vertices) {
  std::vector<VertexOutput> outputs(vertices.size());

  for (std::size_t i = 0; i < vertices.size(); ++i) {
    VertexOutput out{};
    out.position = vertices[i].position * cb.GetWorldMatrix() *
                   cb.GetViewMatrix() * cb.GetProjectionMatrix();
    outputs[i] = out;
  }

  return outputs;
}

Color UnlitColorFragmentShader(const ConstantBuffer& cb, const VertexOutput&) {
  return std::any_cast<Color>(cb.GetProperties()->at("Color_"));
}

Shader UnlitColorShader() { return Shader(UnlitColorFragmentShader); }
}  // namespace softy