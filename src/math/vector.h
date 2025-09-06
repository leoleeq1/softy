#ifndef MATH_VECTOR_H_
#define MATH_VECTOR_H_

#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <limits>
#include <type_traits>

#include "math/math.h"

namespace softy {
template <Arithmetic T, std::size_t N>
struct vec {
  constexpr vec() : v{} {}
  constexpr vec(const vec& rhs) : v{rhs.v} {}
  constexpr vec& operator=(const vec& rhs) {
    v = rhs.v;
    return *this;
  }

  template <Arithmetic U, std::size_t M>
  constexpr vec(const vec<U, M>& rhs) {
    constexpr std::size_t n = min(N, M);
    for (std::size_t i = 0; i < n; ++i) {
      v[i] = static_cast<T>(rhs.v[i]);
    }
  }

  template <Arithmetic U, std::size_t M, typename... Args>
    requires(sizeof...(Args) > 0)
  explicit constexpr vec(const vec<U, M>& rhs, Args... args) {
    static_assert(M + sizeof...(Args) <= N, "Invalid number of arguments!");
    for (std::size_t i = 0; i < M; ++i) {
      v[i] = static_cast<T>(rhs.v[i]);
    }

    std::size_t index = 0;
    ((v[M + index++] = static_cast<T>(args)), ...);
  }

  template <typename... U>
  explicit constexpr vec(U... args)
    requires(sizeof...(U) <= N &&
             (std::is_arithmetic_v<std::decay_t<U>> && ...))
      : v{static_cast<T>(args)...} {}

  template <Arithmetic U, std::size_t M>
  constexpr operator vec<U, M>() {
    vec<U, M> vec{};
    constexpr std::size_t n = min(N, M);
    for (std::size_t i = 0; i < n; ++i) {
      vec.v[i] = static_cast<U>(v[i]);
    }
    return vec;
  }

  constexpr T operator[](std::size_t i) const {
    assert(i < N);
    return v[i];
  }

  constexpr T& operator[](std::size_t i) {
    assert(i < N);
    return v[i];
  }

  static consteval vec Zero() {
    vec<T, N> temp;
    for (std::size_t i = 0; i < N; ++i) {
      temp.v[i] = static_cast<T>(0);
    }
    return temp;
  }

  static consteval vec One() {
    vec<T, N> temp;
    for (std::size_t i = 0; i < N; ++i) {
      temp.v[i] = static_cast<T>(1);
    }
    return temp;
  }

  static consteval vec Basis(std::size_t i) {
    vec<T, N> temp;
    for (std::size_t j = 0; j < N; ++j) {
      temp.v[j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
    }
    return temp;
  }

  std::array<T, N> v;
};

template <Arithmetic T>
struct alignas(alignof(T) * 4) vec<T, 4> {
  constexpr vec() : v{} {}
  constexpr vec(const vec& rhs) : v{rhs.v} {}
  constexpr vec& operator=(const vec& rhs) {
    v = rhs.v;
    return *this;
  }

  template <Arithmetic U, std::size_t M>
  constexpr vec(const vec<U, M>& rhs) {
    constexpr std::size_t n = min(4uz, M);
    for (std::size_t i = 0; i < n; ++i) {
      v[i] = static_cast<T>(rhs.v[i]);
    }
  }

  template <Arithmetic U, std::size_t M, typename... Args>
    requires(sizeof...(Args) > 0)
  explicit constexpr vec(const vec<U, M>& rhs, Args... args) {
    static_assert(M + sizeof...(Args) <= 4, "Invalid number of arguments!");
    for (std::size_t i = 0; i < M; ++i) {
      v[i] = static_cast<T>(rhs.v[i]);
    }

    std::size_t index = 0;
    ((v[M + index++] = static_cast<T>(args)), ...);
  }

  template <typename... U>
  explicit constexpr vec(U... args)
    requires(sizeof...(U) <= 4 &&
             (std::is_arithmetic_v<std::decay_t<U>> && ...))
      : v{static_cast<T>(args)...} {}

  template <Arithmetic U, std::size_t M>
  constexpr operator vec<U, M>() {
    vec<U, M> vec{};
    constexpr std::size_t n = min(4uz, M);
    for (std::size_t i = 0; i < n; ++i) {
      vec.v[i] = static_cast<U>(v[i]);
    }
    return vec;
  }

  constexpr T operator[](std::size_t i) const {
    assert(i < 4);
    return v[i];
  }

  constexpr T& operator[](std::size_t i) {
    assert(i < 4);
    return v[i];
  }

  static consteval vec Zero() {
    vec<T, 4> temp;
    for (std::size_t i = 0; i < 4; ++i) {
      temp.v[i] = static_cast<T>(0);
    }
    return temp;
  }

  static consteval vec One() {
    vec<T, 4> temp;
    for (std::size_t i = 0; i < 4; ++i) {
      temp.v[i] = static_cast<T>(1);
    }
    return temp;
  }

  static consteval vec Basis(std::size_t i) {
    vec<T, 4> temp;
    for (std::size_t j = 0; j < 4; ++j) {
      temp.v[j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
    }
    return temp;
  }

  std::array<T, 4> v;
};

using v2f = vec<float, 2>;
using v3f = vec<float, 3>;
using v4f = vec<float, 4>;

using v2i = vec<int32_t, 2>;
using v3i = vec<int32_t, 3>;
using v4i = vec<int32_t, 4>;

template <Integral T, std::size_t N>
constexpr bool operator==(vec<T, N> lhs, vec<T, N> rhs) {
  return lhs.v == rhs.v;
}

template <Integral T, std::size_t N>
constexpr bool operator!=(vec<T, N> lhs, vec<T, N> rhs) {
  return !operator==(lhs, rhs);
}

template <FloatingPoint T, std::size_t N>
constexpr bool equals(vec<T, N> lhs, vec<T, N> rhs,
                      T epsilon = std::numeric_limits<T>::epsilon(),
                      int32_t maxUlpDiff = 4) {
  assert(maxUlpDiff > 0 && maxUlpDiff <= 4);
  for (std::size_t i = 0; i < N; ++i) {
    if (!equals(lhs[i], rhs[i], epsilon, maxUlpDiff)) {
      return false;
    }
  }

  return true;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator|(vec<T, N> lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] |= rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator&(vec<T, N> lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] &= rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator+(vec<T, N> lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] += rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N>& operator+=(vec<T, N>& lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] += rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator-(vec<T, N> lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] -= rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N>& operator-=(vec<T, N>& lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] -= rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N, Arithmetic U>
constexpr vec<T, N>& operator*=(vec<T, N>& lhs, U rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] *= static_cast<T>(rhs);
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator-(vec<T, N> v) {
  for (std::size_t i = 0; i < N; ++i) {
    v[i] = -v[i];
  }
  return v;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator*(vec<T, N> lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] *= rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator*(Arithmetic auto lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    rhs[i] = lhs * rhs[i];
  }
  return rhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator*(vec<T, N> lhs, Arithmetic auto rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] = static_cast<T>(lhs[i] * rhs);
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator/(vec<T, N> lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    assert(rhs[i] > std::numeric_limits<T>::epsilon() ||
           rhs[i] < -std::numeric_limits<T>::epsilon());
  }
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] /= rhs[i];
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator/(vec<T, N> lhs, Arithmetic auto rhs) {
  assert(rhs > std::numeric_limits<decltype(rhs)>::epsilon() ||
         rhs < -std::numeric_limits<decltype(rhs)>::epsilon());
  for (std::size_t i = 0; i < N; ++i) {
    lhs[i] /= rhs;
  }
  return lhs;
}

template <Arithmetic T, std::size_t N>
constexpr vec<T, N> operator/(Arithmetic auto lhs, vec<T, N> rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    assert(rhs[i] > std::numeric_limits<T>::epsilon() ||
           rhs[i] < -std::numeric_limits<T>::epsilon());
  }

  vec<T, N> v{};
  for (std::size_t i = 0; i < N; ++i) {
    v[i] = lhs / rhs[i];
  }
  return v;
}

template <Arithmetic T, std::size_t N>
constexpr T dot(vec<T, N> lhs, vec<T, N> rhs) {
  T result{};
  for (std::size_t i = 0; i < N; ++i) {
    result += lhs[i] * rhs[i];
  }
  return result;
}

template <Arithmetic T>
constexpr T cross(vec<T, 2> lhs, vec<T, 2> rhs) {
  return (lhs[0] * rhs[1]) - (lhs[1] * rhs[0]);
}

template <Arithmetic T>
constexpr vec<T, 3> cross(vec<T, 3> lhs, vec<T, 3> rhs) {
  return vec<T, 3>{(lhs[1] * rhs[2]) - (lhs[2] * rhs[1]),
                   -((lhs[0] * rhs[2]) - (lhs[2] * rhs[0])),
                   (lhs[0] * rhs[1]) - (lhs[1] * rhs[0])};
}

template <Arithmetic T, std::size_t N>
constexpr auto length(vec<T, N> v) {
  return static_cast<same_size_float<T>::type>(sqrt(dot(v, v)));
}

template <Arithmetic T, std::size_t N>
constexpr T sqrLength(vec<T, N> v) {
  return dot(v, v);
}

template <Arithmetic T, std::size_t N>
constexpr vec<same_size_float_t<T>, N> normalize(vec<T, N> v) {
  auto l = length(v);
  if (isNearlyZero(l)) return vec<same_size_float_t<T>, N>{};
  return vec<same_size_float_t<T>, N>(v) / l;
}

template <Arithmetic T, std::size_t N>
constexpr vec<same_size_float_t<T>, N> reflect(vec<T, N> p, vec<T, N> n) {
  vec<same_size_float_t<T>, N> pn{n * dot(p, n)};
  return vec<same_size_float_t<T>, N>{-p + 2 * pn};
}

template <Arithmetic T, std::size_t N>
constexpr same_size_float_t<T> fraction(vec<T, N> v0, vec<T, N> v1,
                                        vec<T, N> p) {
  vec<T, N> u = v1 - v0;
  vec<T, N> v = p - v0;

  same_size_float_t<T> denom = dot(u, u);
  return dot(v, v) / denom;
}

template <Arithmetic T, std::size_t N>
constexpr vec<same_size_float_t<T>, 3> barycentricCoordinate(vec<T, N> v0,
                                                             vec<T, N> v1,
                                                             vec<T, N> p) {
  vec<T, 3> out{};
  vec<T, N> u = v1 - v0;
  vec<T, N> v = p - v0;

  same_size_float_t<T> denom = dot(u, u);
  out[1] = dot(v, v) / denom;
  out[0] = static_cast<same_size_float_t<T>>(1) - out[1];

  return out;
}

template <Arithmetic T, std::size_t N>
constexpr vec<same_size_float_t<T>, 3> barycentricCoordinate(vec<T, N> v0,
                                                             vec<T, N> v1,
                                                             vec<T, N> v2,
                                                             vec<T, N> p) {
  vec<T, 3> out{};
  vec<T, N> u = v1 - v0;
  vec<T, N> v = v2 - v0;
  vec<T, N> w = p - v0;

  same_size_float_t<T> d00 = dot(u, u);
  same_size_float_t<T> d11 = dot(v, v);
  same_size_float_t<T> d01 = dot(u, v);
  same_size_float_t<T> d20 = dot(w, u);
  same_size_float_t<T> d21 = dot(w, v);

  same_size_float_t<T> denom = (d00 * d11 - (d01 * d01));
  out[1] = (d20 * d11 - d21 * d01) / denom;
  out[2] = (d21 * d00 - d20 * d01) / denom;
  out[0] = static_cast<same_size_float_t<T>>(1) - out[1] - out[2];

  return out;
}
}  // namespace softy

namespace std {
template <softy::Arithmetic T, std::size_t N>
struct std::formatter<softy::vec<T, N>> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const softy::vec<T, N>& v, std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{}", v.v);
  }
};
}  // namespace std

#endif  // MATH_VECTOR_H_