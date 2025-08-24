#ifndef RENDER_MATERIAL_H_
#define RENDER_MATERIAL_H_

#include <any>
#include <string>
#include <unordered_map>

#include "shader/shader.h"

namespace softy {
class Material {
 public:
  Material(Shader* shader) : shader_{shader} {}

  Shader* GetShader() const noexcept { return shader_; }
  const std::unordered_map<std::string, std::any>* GetProperties() const {
    return &properties_;
  }

  void SetProperty(std::string name, std::any value) {
    properties_[name] = value;
  }

 private:
  Shader* shader_;
  std::unordered_map<std::string, std::any> properties_;
};
}  // namespace softy

#endif  // RENDER_MATERIAL_H_