#ifndef MATH_MATRIX_H_
#define MATH_MATRIX_H_

#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include "math/math.h"
#include "math/vector.h"

namespace softy {
template <FloatingPoint T, std::size_t N>
struct mat {
  constexpr mat() : m{} {}
  constexpr mat(const mat& m) : m{m.m} {}

  constexpr explicit mat(std::initializer_list<vec<T, N>> list) : m{} {
    assert(list.size() <= N);
    for (std::size_t r = 0; r < N; ++r) {
      m[r] = list.begin()[r];
    }
  }

  template <std::size_t M>
  constexpr explicit mat(std::initializer_list<vec<T, M>> list) {
    assert(list.size() <= N && M <= N);
    m = Identity().m;
    for (std::size_t r = 0; r < M; ++r) {
      for (std::size_t c = 0; c < M; ++c) {
        m[r][c] = list.begin()[r][c];
      }
    }
  }

  template <typename... U>
  explicit constexpr mat(U... args)
    requires(sizeof...(U) <= (N * N) &&
             (std::is_arithmetic_v<std::decay_t<U>> && ...))
      : m{} {
    std::size_t i = 0;
    ((m[i / N][i % N] = static_cast<T>(args), ++i), ...);
  }

  vec<T, N>& operator[](std::size_t row) {
    assert(row < N);
    return m[row];
  }

  static consteval mat Identity() noexcept {
    mat m{};
    for (std::size_t i = 0; i < N; ++i) {
      m.m[i][i] = static_cast<T>(1);
    }
    return m;
  }

  std::array<vec<T, N>, N> m{};
};

template <FloatingPoint T>
struct alignas(alignof(T) * 4) mat<T, 4> {
  constexpr mat() : m{} {}
  constexpr mat(const mat& m) : m{m.m} {}

  constexpr explicit mat(std::initializer_list<vec<T, 4>> list) : m{} {
    assert(list.size() <= 4);
    for (std::size_t r = 0; r < 4; ++r) {
      m[r] = list.begin()[r];
    }
  }

  template <std::size_t M>
  constexpr explicit mat(std::initializer_list<vec<T, M>> list) {
    assert(list.size() <= 4 && M <= 4);
    m = Identity().m;
    for (std::size_t r = 0; r < M; ++r) {
      for (std::size_t c = 0; c < M; ++c) {
        m[r][c] = list.begin()[r][c];
      }
    }
  }

  template <typename... U>
  explicit constexpr mat(U... args)
    requires(sizeof...(U) <= (4 * 4) &&
             (std::is_arithmetic_v<std::decay_t<U>> && ...))
      : m{} {
    std::size_t i = 0;
    ((m[i / 4][i % 4] = static_cast<T>(args), ++i), ...);
  }

  vec<T, 4>& operator[](std::size_t row) {
    assert(row < 4);
    return m[row];
  }

  static consteval mat Identity() noexcept {
    mat m{};
    for (std::size_t i = 0; i < 4; ++i) {
      m.m[i][i] = static_cast<T>(1);
    }
    return m;
  }

  std::array<vec<T, 4>, 4> m{};
};

using mat4 = mat<float, 4>;

template <FloatingPoint T, std::size_t N>
constexpr mat<T, N> Transpose(mat<T, N> m) {
  for (std::size_t r = 1; r < N; ++r) {
    for (std::size_t c = 0; c < r; ++c) {
      std::swap(m[r][c], m[c][r]);
    }
  }
  return m;
}

template <FloatingPoint T, std::size_t N>
constexpr mat<T, N> operator+(mat<T, N> lhs, mat<T, N> rhs) {
  for (std::size_t r = 0; r < N; ++r) {
    for (std::size_t c = 0; c < N; ++c) {
      lhs[r][c] += rhs[r][c];
    }
  }
  return lhs;
}

template <FloatingPoint T, std::size_t N>
constexpr mat<T, N> operator-(mat<T, N> lhs, mat<T, N> rhs) {
  for (std::size_t r = 0; r < N; ++r) {
    for (std::size_t c = 0; c < N; ++c) {
      lhs[r][c] -= rhs[r][c];
    }
  }
  return lhs;
}

template <typename T, std::size_t N>
constexpr mat<T, N> operator*(mat<T, N> lhs, mat<T, N> rhs) {
  mat<T, N> m{};
  rhs = Transpose(rhs);
  for (std::size_t r = 0; r < N; ++r) {
    for (std::size_t q = 0; q < N; ++q) {
      m[r][q] = dot(lhs[r], rhs[q]);
    }
  }
  return m;
}

template <typename T, std::size_t N>
constexpr vec<T, N> operator*(vec<T, N> lhs, mat<T, N> rhs) {
  vec<T, N> v{};
  rhs = Transpose(rhs);
  for (std::size_t r = 0; r < N; ++r) {
    v[r] = dot(lhs, rhs[r]);
  }
  return v;
}

template <typename T, std::size_t N>
constexpr mat<T, N> operator/(mat<T, N> lhs, Arithmetic auto rhs) {
  for (std::size_t r = 0; r < N; ++r) {
    lhs[r] /= rhs;
  }
  return lhs;
}

template <typename T, std::size_t N>
constexpr mat<T, N> operator/(Arithmetic auto lhs, mat<T, N> rhs) {
  for (std::size_t r = 0; r < N; ++r) {
    rhs[r] = lhs / rhs[r];
  }
  return rhs;
}
}  // namespace softy

namespace std {
template <softy::FloatingPoint T, std::size_t N>
struct std::formatter<softy::mat<T, N>> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const softy::mat<T, N>& m, std::format_context& ctx) const {
    auto out = ctx.out();
    for (std::size_t i = 0; i < N; ++i) {
      out = std::format_to(out, "{}", m.m[i]);
      if (i != (N - 1)) {
        *out++ = '\n';
      }
    }
    return out;
  }
};
}  // namespace std

#endif  // MATH_MATRIX_H_