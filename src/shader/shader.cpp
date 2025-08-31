#include "shader/shader.h"

#include <any>
#include <print>

namespace softy {
VertexOutput softy::DefaultVertexShader(const ConstantBuffer& cb,
                                        const Vertex& vertex) {
  VertexOutput output;

  output.position = vertex.position * cb.GetWorldMatrix() * cb.GetViewMatrix() *
                    cb.GetProjectionMatrix();
  output.normal = vertex.normal;
  output.uv = vertex.uv;
  output.color = vertex.color;

  return output;
}

Color UnlitColorFragmentShader(const ConstantBuffer& cb,
                               const VertexOutput& v) {
  return v.color * std::any_cast<Color>(cb.GetProperties()->at("Color_"));
}

Shader UnlitColorShader() { return Shader(UnlitColorFragmentShader); }
}  // namespace softy