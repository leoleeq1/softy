#include "render/camera.h"

#include "camera.h"
#include "math/math.h"
#include "math/vector.h"
#include "render/buffer.h"

namespace softy {
float Camera::GetAspect() const noexcept {
  return renderTarget_->GetWidth() /
         static_cast<float>(renderTarget_->GetHeight());
  ;
}

mat4 Camera::GetViewMatrix() const noexcept {
  mat4 t = mat4{
      v4f::Basis(0),
      v4f::Basis(1),
      v4f::Basis(2),
      v4f{-transform_.position, 1.0f},
  };
  mat4 r = Transpose(mat4{
      v4f{v3f{transform_.right}},
      v4f{v3f{transform_.up}},
      v4f{v3f{transform_.forward}},
      v4f::Basis(3),
  });
  return t * r;
}

mat4 Camera::GetProjectionMatrix() const noexcept {
  float rAspect = renderTarget_->GetHeight() /
                  static_cast<float>(renderTarget_->GetWidth());
  float d = 1.0f / tan(numbers::fDeg2Rad * fov_ * 0.5f);
  return mat4{
      v4f{d * rAspect, 0.0f, 0.0f, 0.0f},
      v4f{0.0f, d, 0.0f, 0.0f},
      v4f{0.0f, 0.0f, -(near_ + far_) / (far_ - near_), -1.0f},
      v4f{0.0f, 0.0f, -(2.0f * near_ * far_) / (far_ - near_), 0.0f},
  };
}
}  // namespace softy
