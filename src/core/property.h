#ifndef CORE_PROPERTY_H_
#define CORE_PROPERTY_H_

#include <cassert>
#include <functional>

#include "math/math.h"

namespace softy {
template <typename T>
class Property {
  using Getter = std::function<T(void)>;
  using Setter = std::function<void(const T&)>;

 public:
  constexpr Property(Getter getter);
  constexpr Property(Getter getter, Setter setter);
  constexpr Property(const Property&) = delete;
  constexpr Property& operator=(const Property&) = delete;

  constexpr void SetProperty(Getter getter, Setter setter);

  constexpr T Get() const;
  constexpr void Set(const T& value) const;

  constexpr void operator=(const T& v) const;
  constexpr auto operator<=>(const Property& rhs) const;
  constexpr operator T() const;

 private:
  Getter getter_{};
  Setter setter_{};
};

template <typename T>
constexpr Property<T>::Property(Getter getter) : getter_{getter} {
  assert(getter != nullptr);
}

template <typename T>
constexpr Property<T>::Property(Getter getter, Setter setter)
    : getter_{getter}, setter_{setter} {
  assert(getter != nullptr);
}

template <typename T>
constexpr void Property<T>::SetProperty(Getter getter, Setter setter) {
  assert(getter != nullptr);
  getter_ = getter;
  setter_ = setter;
}

template <typename T>
constexpr T Property<T>::Get() const {
  assert(getter_ != nullptr);
  return getter_();
}

template <typename T>
constexpr void Property<T>::Set(const T& value) const {
  assert(setter_ != nullptr);
  setter_(value);
}

template <typename T>
constexpr void Property<T>::operator=(const T& v) const {
  setter_(v);
}

template <typename T>
constexpr auto Property<T>::operator<=>(const Property& rhs) const {
  return getter_() <=> rhs.getter_();
}

template <typename T>
constexpr Property<T>::operator T() const {
  return getter_();
}

template <HasArithmeticOp T>
constexpr T operator+(const T& lhs, const Property<T>& rhs) {
  return lhs + T(rhs);
}

template <HasArithmeticOp T>
constexpr T operator+(const Property<T>& lhs, const T& rhs) {
  return T(lhs) + rhs;
}

template <HasArithmeticOp T>
constexpr T operator-(const T& lhs, const Property<T>& rhs) {
  return lhs - T(rhs);
}

template <HasArithmeticOp T>
constexpr T operator-(const Property<T>& lhs, const T& rhs) {
  return T(lhs) - rhs;
}

template <HasArithmeticOp T>
constexpr T operator*(const T& lhs, const Property<T>& rhs) {
  return lhs * T(rhs);
}

template <HasArithmeticOp T>
constexpr T operator*(const Property<T>& lhs, const T& rhs) {
  return T(lhs) * rhs;
}

template <HasDivisionOp T>
constexpr T operator/(const T& lhs, const Property<T>& rhs) {
  return lhs / T(rhs);
}

template <HasDivisionOp T>
constexpr T operator/(const Property<T>& lhs, const T& rhs) {
  return T(lhs) / rhs;
}

template <HasModuloOp T>
constexpr T operator%(const T& lhs, const Property<T>& rhs) {
  return lhs % T(rhs);
}

template <HasModuloOp T>
constexpr T operator%(const Property<T>& lhs, const T& rhs) {
  return T(lhs) % rhs;
}

template <typename T>
constexpr auto operator<=>(const T& lhs, const Property<T>& rhs) {
  return lhs <=> T(rhs);
}

template <typename T>
constexpr auto operator<=>(const Property<T>& lhs, const T& rhs) {
  return T(lhs) <=> rhs;
}
}  // namespace softy

#endif  // CORE_PROPERTY_H_