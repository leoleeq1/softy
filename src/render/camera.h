#ifndef RENDER_CAMERA_H_
#define RENDER_CAMERA_H_

#include "core/transform.h"
#include "math/vector.h"
#include "render/buffer.h"

namespace softy {
class Camera {
 public:
  Camera(ColorBuffer* renderTarget) : renderTarget_{renderTarget} {}

  float GetAspect() const noexcept;
  mat4 GetViewMatrix() const noexcept;
  mat4 GetProjectionMatrix() const noexcept;
  ColorBuffer* GetRenderTarget() const noexcept { return renderTarget_; }
  Transform& GetTransform() noexcept { return transform_; }

 private:
  Transform transform_;
  ColorBuffer* renderTarget_{nullptr};
  float far_{1000.0f};
  float near_{1.0f};
  float fov_{60.0f};
};
}  // namespace softy

#endif  // RENDER_CAMERA_H_