#ifndef CORE_TRANSFORM_H_
#define CORE_TRANSFORM_H_

#include <vector>

#include "core/property.h"
#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"

namespace softy {
class Transform {
 public:
  Transform();
  Transform(const Transform& t) = delete;
  Transform& operator=(const Transform& t) = delete;

  Property<v3f> position;
  Property<v3f> rotation;
  Property<v3f> scale;

  Property<v3f> right;
  Property<v3f> up;
  Property<v3f> forward;

  [[nodiscard]] v3f GetWorldPosition() const noexcept;
  [[nodiscard]] v3f GetWorldRotation() const noexcept;
  [[nodiscard]] v3f GetWorldScale() const noexcept;

  [[nodiscard]] mat4 GetLocalTRS() const noexcept;
  [[nodiscard]] mat4 GetLocalInverseTRS() const noexcept;

  [[nodiscard]] mat4 GetTRS() const noexcept;
  [[nodiscard]] mat4 GetInverseTRS() const noexcept;

  [[nodiscard]] Transform* GetParent() const noexcept;
  [[nodiscard]] Transform* GetChild(std::size_t i) const noexcept;
  [[nodiscard]] const std::vector<Transform*>& GetChildren() const noexcept;
  [[nodiscard]] std::size_t GetChildrenSize() const noexcept;

  void SetParent(Transform* parent);

  void SetLocalPositionRotation(v3f position, v3f rotation) noexcept;
  void SetWorldPositionRotation(v3f position, v3f rotation) noexcept;

  void LookAt(v3f target, bool inverse = false) noexcept;

  [[nodiscard]] static inline mat4 GetTranslateMatrix(v3f position) noexcept;
  [[nodiscard]] static inline mat4 GetRotationMatrix(v3f rotation) noexcept;
  [[nodiscard]] static inline mat4 GetScaleMatrix(v3f scale) noexcept;
  [[nodiscard]] static inline mat4 GetInverseScaleMatrix(v3f scale) noexcept;

 private:
  void CalculateAxes() noexcept;
  void CalculateMatrices() noexcept;

  void Update() noexcept;

  mat4 worldTrs_{mat4::Identity()};
  mat4 invWorldTrs_{mat4::Identity()};

  v3f worldPosition_{};
  v3f worldRotation_{};
  v3f worldScale_{v3f::One()};

  v3f localPosition_{};
  v3f localRotation_{};
  v3f localScale_{v3f::One()};

  v3f right_{v3f::Basis(0)};
  v3f up_{v3f::Basis(1)};
  v3f forward_{v3f::Basis(2)};

  std::vector<Transform*> children_;
  Transform* parent_{nullptr};
};

inline mat4 Transform::GetTranslateMatrix(v3f position) noexcept {
  return mat4{
      v4f::Basis(0),
      v4f::Basis(1),
      v4f::Basis(2),
      v4f{position, 1.0f},
  };
}

inline mat4 Transform::GetRotationMatrix(v3f rotation) noexcept {
  float cp = cos(numbers::fDeg2Rad * rotation[0]);
  float sp = sin(numbers::fDeg2Rad * rotation[0]);
  float cy = cos(numbers::fDeg2Rad * rotation[1]);
  float sy = sin(numbers::fDeg2Rad * rotation[1]);
  float cr = cos(numbers::fDeg2Rad * rotation[2]);
  float sr = sin(numbers::fDeg2Rad * rotation[2]);

  return mat4{
      v4f{cr * cy + sr * sp * sy, -sr * cy + cr * sp * sy, cp * sy, 0.0f},
      v4f{sr * cp, cr * cp, -sp, 0.0f},
      v4f{-sy * cr + sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, 0.0f},
      v4f::Basis(3),
  };
}

inline mat4 Transform::GetScaleMatrix(v3f scale) noexcept {
  return mat4{
      v4f::Basis(0) * scale[0],
      v4f::Basis(1) * scale[1],
      v4f::Basis(2) * scale[2],
      v4f::Basis(3),
  };
}

inline mat4 Transform::GetInverseScaleMatrix(v3f scale) noexcept {
  return mat4{
      v4f::Basis(0) / scale[0],
      v4f::Basis(1) / scale[1],
      v4f::Basis(2) / scale[2],
      v4f::Basis(3),
  };
}
}  // namespace softy

#endif  // CORE_TRANSFORM_H_