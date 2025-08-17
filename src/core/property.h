#ifndef CORE_PROPERTY_H_
#define CORE_PROPERTY_H_

#include <functional>

namespace softy {
template <typename T>
class Property {
  using Getter = std::function<T(const T&)>;
  using Setter = std::function<void(const T&)>;

 public:
  Property() = default;
  Property(const T& value, Getter getter = nullptr, Setter setter = nullptr);
  Property(const Property<T>& rhs);
  Property<T>& operator=(const Property<T>& rhs);

  void SetGetter(Getter getter);
  void SetSetter(Setter setter);
  void SetProperty(Getter getter, Setter setter);
  void SetValueWithoutNotify(const T& value);

  Property<T> operator+(const T& value) const;
  Property<T> operator-(const T& value) const;
  Property<T> operator*(const T& value) const;
  Property<T> operator/(const T& value) const;
  Property<T> operator%(const T& value) const;
  Property<T>& operator=(const T& value);
  Property<T>& operator+=(const T& value);
  Property<T>& operator-=(const T& value);
  Property<T>& operator*=(const T& value);
  Property<T>& operator/=(const T& value);
  Property<T> operator+() const;
  Property<T> operator-() const;
  Property<T>& operator++();
  Property<T> operator++(int);
  Property<T>& operator--();
  Property<T> operator--(int);
  bool operator!() const;
  auto operator<=>(const T& value) const;
  auto operator<=>(const Property& rhs) const;
  operator T() const;

 private:
  Getter getter_{};
  Setter setter_{};
  T value_{};
};

template <typename T>
inline Property<T>::Property(const T& value, Getter getter, Setter setter)
    : value_{value}, getter_{getter}, setter_{setter} {}

template <typename T>
inline Property<T>::Property(const Property& rhs)
    : value_{rhs.value_}, getter_{rhs.getter_}, setter_{rhs.setter_} {}

template <typename T>
inline Property<T>& Property<T>::operator=(const Property<T>& rhs) {
  value_ = rhs.value_;
  getter_ = rhs.getter_;
  setter_ = rhs.setter_;
}

template <typename T>
inline void Property<T>::SetGetter(Getter getter) {
  getter_ = getter;
}

template <typename T>
inline void Property<T>::SetSetter(Setter setter) {
  setter_ = setter;
}

template <typename T>
inline void Property<T>::SetProperty(Getter getter, Setter setter) {
  getter_ = getter;
  setter_ = setter;
}

template <typename T>
inline void Property<T>::SetValueWithoutNotify(const T& value) {
  value_ = value;
}

template <typename T>
inline Property<T> Property<T>::operator+(const T& value) const {
  return Property<T>{value_ + value, getter_, setter_};
}

template <typename T>
inline Property<T> Property<T>::operator-(const T& value) const {
  return Property<T>{value_ - value, getter_, setter_};
}

template <typename T>
inline Property<T> Property<T>::operator*(const T& value) const {
  return Property<T>{value_ * value, getter_, setter_};
}

template <typename T>
inline Property<T> Property<T>::operator/(const T& value) const {
  return Property<T>{value_ / value, getter_, setter_};
}

template <typename T>
inline Property<T> Property<T>::operator%(const T& value) const {
  return Property<T>{value_ % value, getter_, setter_};
}

template <typename T>
inline Property<T>& Property<T>::operator=(const T& value) {
  value_ = value;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return *this;
}

template <typename T>
inline Property<T>& Property<T>::operator+=(const T& value) {
  value_ += value;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return *this;
}

template <typename T>
inline Property<T>& Property<T>::operator-=(const T& value) {
  value_ -= value;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return *this;
}

template <typename T>
inline Property<T>& Property<T>::operator*=(const T& value) {
  value_ *= value;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return *this;
}

template <typename T>
inline Property<T>& Property<T>::operator/=(const T& value) {
  value_ /= value;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return *this;
}

template <typename T>
inline Property<T> Property<T>::operator+() const {
  return Property<T>{+value_, getter_, setter_};
}

template <typename T>
inline Property<T> Property<T>::operator-() const {
  return Property<T>{-value_, getter_, setter_};
}

template <typename T>
inline Property<T>& Property<T>::operator++() {
  ++value_;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return *this;
}

template <typename T>
inline Property<T> Property<T>::operator++(int) {
  T temp{value_};
  ++value_;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return Property<T>{temp, getter_, setter_};
}

template <typename T>
inline Property<T>& Property<T>::operator--() {
  --value_;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return *this;
}

template <typename T>
inline Property<T> Property<T>::operator--(int) {
  T temp{value_};
  --value_;
  if (setter_ != nullptr) {
    setter_(value_);
  }
  return Property<T>{temp, getter_, setter_};
}

template <typename T>
inline bool Property<T>::operator!() const {
  return !value_;
}

template <typename T>
inline auto Property<T>::operator<=>(const T& value) const {
  return value_ <=> value;
}

template <typename T>
inline auto operator<=>(const T& value, const Property<T>& prop) {
  return value <=> prop;
}

template <typename T>
inline auto Property<T>::operator<=>(const Property& rhs) const {
  return value_ <=> rhs.value_;
}

template <typename T>
inline Property<T>::operator T() const {
  if (getter_ != nullptr) {
    return getter_(value_);
  }
  return value_;
}
}  // namespace softy

#endif  // CORE_PROPERTY_H_