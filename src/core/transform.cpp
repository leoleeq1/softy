#include "core/transform.h"

#include <algorithm>
#include <cassert>

#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "transform.h"

namespace softy {
Transform::Transform()
    : position{[&]() { return localPosition_; },
               [&](auto& v) {
                 localPosition_ = v;
                 Update();
               }},
      rotation{[&]() { return localRotation_; },
               [&](auto& v) {
                 localRotation_ = v;
                 Update();
               }},
      scale{[&]() { return localScale_; },
            [&](auto& v) {
              localScale_ = v;
              Update();
            }},
      right{[&]() { return right_; }},
      up{[&]() { return up_; }},
      forward{[&]() { return forward_; }} {}

v3f Transform::GetWorldPosition() const noexcept { return worldPosition_; }

v3f Transform::GetWorldRotation() const noexcept { return worldRotation_; }

v3f Transform::GetWorldScale() const noexcept { return worldScale_; }

mat4 Transform::GetLocalTRS() const noexcept {
  mat4 t{GetTranslateMatrix(localPosition_)};
  mat4 r{GetRotationMatrix(localRotation_)};
  mat4 s{GetScaleMatrix(localScale_)};
  return s * r * t;
}

mat4 Transform::GetLocalInverseTRS() const noexcept {
  mat4 t{mat4{
      v4f::Basis(0),
      v4f::Basis(1),
      v4f::Basis(2),
      v4f{-localPosition_, 1.0f},
  }};
  mat4 r{Transpose(GetRotationMatrix(localRotation_))};
  mat4 s{GetInverseScaleMatrix(localScale_)};
  return s * r * t;
}

mat4 Transform::GetTRS() const noexcept { return worldTrs_; }

mat4 Transform::GetInverseTRS() const noexcept { return invWorldTrs_; }

Transform* Transform::GetParent() const noexcept { return parent_; }

Transform* Transform::GetChild(std::size_t i) const noexcept {
  assert(i >= children_.size());
  return children_[i];
}

const std::vector<Transform*>& Transform::GetChildren() const noexcept {
  return children_;
}

std::size_t Transform::GetChildrenSize() const noexcept {
  return children_.size();
}

void Transform::SetParent(Transform* parent) {
  for (Transform* cur = parent; cur != nullptr; cur = cur->parent_) {
    if (cur == this) {
      return;
    }
  }

  if (parent_ != nullptr) {
    auto it = std::ranges::find(parent_->children_, this);
    assert(it == parent_->children_.end());
    parent_->children_.erase(it);
    parent_ = nullptr;
  }

  if (parent == nullptr) {
    localPosition_ = worldPosition_;
    localRotation_ = worldRotation_;
    localScale_ = worldScale_;
    Update();
    return;
  }

  parent_ = parent;
  parent_->children_.push_back(this);

  localPosition_ = v4f{localPosition_, 1.0f} * parent_->GetInverseTRS();
  localRotation_ = localRotation_ - parent->worldRotation_;
  localScale_ = localScale_ / parent_->worldScale_;

  Update();
}

void Transform::SetLocalPositionRotation(v3f position, v3f rotation) noexcept {
  localPosition_ = position;
  localRotation_ = rotation;
  Update();
}

void Transform::SetWorldPositionRotation(v3f position, v3f rotation) noexcept {
  worldPosition_ = position;
  worldRotation_ = rotation;

  localPosition_ = parent_ == nullptr
                       ? position
                       : v3f{v4f{position} * parent_->GetInverseTRS()};
  localRotation_ = parent_ == nullptr
                       ? worldRotation_
                       : worldRotation_ - parent_->worldRotation_;
  Update();
}

void Transform::LookAt(v3f target, bool inverse) noexcept {
  v3f forward{normalize(inverse ? position - target : target - position)};
  v3f unitY{v3f::Basis(1)};
  v3f right{equals(unitY, forward) || equals(-unitY, forward)
                ? v3f::Basis(0)
                : cross(forward, unitY)};
  v3f up{cross(right, forward)};

  v3f rotation{};
  rotation[0] = clampDegree360(numbers::fRad2Deg * asin(-forward[1]));
  float cp = cos(numbers::fDeg2Rad * rotation[0]);
  rotation[1] = clampDegree360(numbers::fRad2Deg * acos(forward[2] / cp));
  rotation[2] = clampDegree360(numbers::fRad2Deg * asin(right[1] / cp));

  this->rotation = rotation;
}

void Transform::CalculateAxes() noexcept {
  mat4 rot{GetRotationMatrix(worldRotation_)};
  right_ = v3f{v4f{v3f::Basis(0), 1.0f} * rot};
  up_ = v3f{v4f{v3f::Basis(1), 1.0f} * rot};
  forward_ = v3f{v4f{v3f::Basis(2), 1.0f} * rot};
}

void Transform::CalculateMatrices() noexcept {
  mat4 trs{GetLocalTRS()};
  mat4 invTrs{GetLocalInverseTRS()};
  if (parent_ == nullptr) {
    worldPosition_ = localPosition_;
    worldRotation_ = localRotation_;
    worldScale_ = localScale_;
    worldTrs_ = trs;
    invWorldTrs_ = invTrs;
  } else {
    worldPosition_ = v4f{localPosition_} * parent_->worldTrs_;
    worldRotation_ = localRotation_ + parent_->localRotation_;
    worldScale_ = localScale_ * parent_->worldScale_;
    worldTrs_ = trs * parent_->worldTrs_;
    invWorldTrs_ = invTrs * parent_->invWorldTrs_;
  }
}

void Transform::Update() noexcept {
  CalculateAxes();
  CalculateMatrices();

  for (Transform* child : children_) {
    child->Update();
  }
}
}  // namespace softy
