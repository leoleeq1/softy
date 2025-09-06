#ifndef MATH_QUATERNION_H_
#define MATH_QUATERNION_H_

#include <cassert>

#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"

namespace softy {
struct quaternion {
  constexpr quaternion() {}
  constexpr explicit quaternion(v4f quat) : value{quat} {}
  constexpr explicit quaternion(v3f rotation) {
    const float cp = cos(numbers::fDeg2Rad * rotation[0] * 0.5f);
    const float sp = sin(numbers::fDeg2Rad * rotation[0] * 0.5f);
    const float cy = cos(numbers::fDeg2Rad * rotation[1] * 0.5f);
    const float sy = sin(numbers::fDeg2Rad * rotation[1] * 0.5f);
    const float cr = cos(numbers::fDeg2Rad * rotation[2] * 0.5f);
    const float sr = sin(numbers::fDeg2Rad * rotation[2] * 0.5f);

    value = v4f{
        cy * sp * cr + sy * cp * sr,
        sy * cp * cr - cy * sp * sr,
        cy * cp * sr - sy * sp * cr,
        cy * cp * cr + sy * sp * sr,
    };
  }

  template <typename... Args>
  constexpr explicit quaternion(Args... args) : value{args...} {}

  constexpr float operator[](std::size_t i) const { return value[i]; }
  constexpr float& operator[](std::size_t i) { return value[i]; }

  constexpr operator mat4() const {
    const float xs = value[0] * value[0];
    const float ys = value[1] * value[1];
    const float zs = value[2] * value[2];
    const float wx = value[3] * value[0];
    const float wy = value[3] * value[1];
    const float wz = value[3] * value[2];
    const float xy = value[0] * value[1];
    const float xz = value[0] * value[2];
    const float yz = value[1] * value[2];

    return mat4{
        v4f{1.0f - 2.0f * (ys + zs), 2.0f * (xy - wz), 2.0f * (wy + xz), 0.0f},
        v4f{2.0f * (xy + wz), 1.0f - 2.0f * (xs + zs), 2.0f * (yz - wx), 0.0f},
        v4f{2.0f * (xz - wy), 2.0f * (wx + yz), 1.0f - 2.0f * (xs + ys), 0.0f},
        v4f{0.0f, 0.0f, 0.0f, 1.0f}};
  }

  static consteval quaternion Identity() noexcept { return quaternion{}; }

  v4f value{0.0f, 0.0f, 0.0f, 1.0f};
};

constexpr quaternion operator+(quaternion lhs, quaternion rhs) {
  lhs.value += rhs.value;
  return lhs;
}

constexpr quaternion& operator+=(quaternion& lhs, quaternion rhs) {
  lhs.value += rhs.value;
  return lhs;
}

constexpr quaternion operator-(quaternion lhs, quaternion rhs) {
  lhs.value -= rhs.value;
  return lhs;
}

constexpr quaternion& operator-=(quaternion& lhs, quaternion rhs) {
  lhs.value -= rhs.value;
  return lhs;
}

constexpr quaternion operator*(float value, quaternion q) {
  return quaternion{q.value * value};
}

constexpr quaternion operator*(quaternion q, float value) {
  return quaternion{q.value * value};
}

constexpr quaternion& operator*=(quaternion& q, float value) {
  q.value *= value;
  return q;
}

constexpr quaternion operator*(quaternion lhs, quaternion rhs) {
  v3f v0{lhs.value};
  v3f v1{rhs.value};
  v3f v{v1 * lhs[3] + v0 * rhs[3] + cross(v0, v1)};

  float w{lhs[3] * rhs[3] - dot(v0, v1)};
  return quaternion{v[0], v[1], v[2], w};
}

constexpr quaternion operator*=(quaternion& lhs, quaternion rhs) {
  v3f v0{lhs.value};
  v3f v1{rhs.value};
  v3f v{v1 * lhs[3] + v0 * rhs[3] + cross(v0, v1)};

  float w{lhs[3] * rhs[3] - dot(v0, v1)};
  lhs.value = v4f{v[0], v[1], v[2], w};
  return lhs;
}

constexpr v3f operator*(quaternion q, v3f v) {
  v3f qv{q.value};
  v3f t{cross(qv, v) * 2.0f};
  return (v + t * q[3] + cross(qv, t));
}

constexpr quaternion normalize(quaternion q) {
  const float sqrLen = sqrLength(q.value);
  if (sqrLen >= numbers::fKindOfSmallNumber) {
    const float inv = 1.0f / sqrt(sqrLen);
    q.value *= inv;
    return q;
  }

  return quaternion::Identity();
}

constexpr quaternion lerp(quaternion lhs, quaternion rhs, float t) {
  quaternion result{lhs * (1.0f - t) + rhs * t};
  result = normalize(result);
  return result;
}

constexpr quaternion slerp(quaternion lhs, quaternion rhs, float t) {
  const float cosTheta = clamp(dot(lhs.value, rhs.value), -1.0f, 1.0f);

  if (cosTheta <= -0.99999f || cosTheta >= 0.99999f) {
    return lerp(lhs, rhs, t);
  }

  const float theta = acos(cosTheta);
  const float invSin = 1.0f / sin(theta);
  const float s0 = sin((1.0f - t) * theta) * invSin;
  const float s1 = sin(t * theta) * invSin;

  return quaternion{lhs * s0 + rhs * s1};
}

constexpr v3f GetForwardVector(quaternion q) { return q * v3f::Basis(2); }

constexpr v3f ToEuler(quaternion q) {
  const float pitchHalfRad = q[3] * q[0] - q[1] * q[2];
  const float yawY = 2.0f * (q[3] * q[1] + q[0] * q[2]);
  const float yawX = 1.0f - 2.0f * (q[0] * q[0] + q[1] * q[1]);

  const float yaw = numbers::fRad2Deg * atan2(yawY, yawX);

  const float singularity = 0.499999f;
  if (pitchHalfRad > singularity) {
    return v3f{90.0f, yaw, yaw - numbers::fRad2Deg * 2.0f * atan2(q[1], q[3])};
  }

  if (pitchHalfRad < -singularity) {
    return v3f{-90.0f, yaw,
               -yaw + numbers::fRad2Deg * 2.0f * atan2(q[1], q[3])};
  }

  return v3f{
      numbers::fRad2Deg * asin(2.0f * pitchHalfRad), yaw,
      numbers::fRad2Deg * atan2(2.0f * (q[3] * q[2] + q[0] * q[1]),
                                1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]))};
}
}  // namespace softy

#endif  // MATH_QUATERNION_H_