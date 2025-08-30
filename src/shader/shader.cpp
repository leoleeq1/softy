#include "shader/shader.h"

#include <any>

namespace softy {
VertexOutput softy::DefaultVertexShader(const ConstantBuffer& cb,
                                        const Vertex& vertex) {
  VertexOutput output;

  output.position = vertex.position * cb.GetWorldMatrix() * cb.GetViewMatrix() *
                    cb.GetProjectionMatrix();

  return output;
}

Color UnlitColorFragmentShader(const ConstantBuffer& cb, const VertexOutput&) {
  return std::any_cast<Color>(cb.GetProperties()->at("Color_"));
}

Shader UnlitColorShader() { return Shader(UnlitColorFragmentShader); }
}  // namespace softy