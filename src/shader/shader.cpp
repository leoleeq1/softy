#include "shader/shader.h"

#include <any>

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

Color UvColorFragmentShader(const ConstantBuffer& cb, const VertexOutput& v) {
  return Color{v.uv[0], v.uv[1], 0.0f};
}

Color VertexColorFragmentShader(const ConstantBuffer& cb,
                                const VertexOutput& v) {
  return v.color * std::any_cast<Color>(cb.GetProperties()->at("Color_"));
}

Color UnlitColorFragmentShader(const ConstantBuffer& cb,
                               const VertexOutput& v) {
  return std::any_cast<Color>(cb.GetProperties()->at("Color_"));
}

Shader UvColorShader() { return Shader(UvColorFragmentShader); }
Shader VertexColorShader() { return Shader(VertexColorFragmentShader); }
Shader UnlitColorShader() { return Shader(UnlitColorFragmentShader); }
}  // namespace softy