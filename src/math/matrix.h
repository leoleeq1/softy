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
  constexpr mat& operator=(const mat& rhs) {
    m = rhs.m;
    return *this;
  }

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

  template <std::size_t M>
  constexpr explicit mat(mat<T, M> mat) {
    std::size_t n = min(N, M);
    for (std::size_t r = 0; r < n; ++r) {
      for (std::size_t c = 0; c < n; ++c) {
        m[r][c] = mat.m[r][c];
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

  constexpr vec<T, N>& operator[](std::size_t row) {
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
  constexpr mat& operator=(const mat& rhs) {
    m = rhs.m;
    return *this;
  }

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

  template <std::size_t M>
  constexpr explicit mat(mat<T, M> mat) {
    std::size_t n = min(4, M);
    for (std::size_t r = 0; r < n; ++r) {
      for (std::size_t c = 0; c < n; ++c) {
        m[r][c] = mat.m[r][c];
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

  constexpr vec<T, 4>& operator[](std::size_t row) {
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

using mat2 = mat<float, 2>;
using mat3 = mat<float, 3>;
using mat4 = mat<float, 4>;

template <FloatingPoint T, std::size_t N>
constexpr bool equals(mat<T, N> lhs, mat<T, N> rhs,
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
constexpr vec<T, N> operator*(mat<T, N> lhs, vec<T, N> rhs) {
  vec<T, N> v{};
  for (std::size_t r = 0; r < N; ++r) {
    v[r] = dot(lhs[r], rhs);
  }
  return v;
}

template <typename T, std::size_t N>
constexpr mat<T, N> operator*(mat<T, N> lhs, Arithmetic auto rhs) {
  for (std::size_t r = 0; r < N; ++r) {
    lhs[r] *= rhs;
  }
  return lhs;
}

template <typename T, std::size_t N>
constexpr mat<T, N> operator*(Arithmetic auto lhs, mat<T, N> rhs) {
  for (std::size_t r = 0; r < N; ++r) {
    rhs[r] = lhs * rhs[r];
  }
  return rhs;
}

template <typename T, std::size_t N>
constexpr mat<T, N> operator/(mat<T, N> lhs, Arithmetic auto rhs) {
  for (std::size_t r = 0; r < N; ++r) {
    lhs[r] /= rhs;
  }
  return lhs;
}

template <typename T, std::size_t N>
constexpr mat<T, N>& operator/=(mat<T, N>& lhs, Arithmetic auto rhs) {
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

template <FloatingPoint T>
constexpr mat<T, 2> Inverse(mat<T, 2> m) {
  T det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
  assert(det != 0);
  return mat<T, 2>{m[1][1], -m[0][1], -m[1][0], m[0][0]} / det;
}

template <FloatingPoint T>
constexpr mat<T, 3> Inverse(mat<T, 3> m) {
  T det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
          m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
          m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

  assert(det != 0);

  mat<T, 3> inv;
  inv[0] = vec<T, 3>{+(m[1][1] * m[2][2] - m[2][1] * m[1][2]),
                     -(m[1][0] * m[2][2] - m[2][0] * m[1][2]),
                     +(m[1][0] * m[2][1] - m[2][0] * m[1][1])};
  inv[1] = vec<T, 3>{+(m[0][1] * m[2][2] - m[2][1] * m[0][2]),
                     -(m[0][0] * m[2][2] - m[2][0] * m[0][2]),
                     +(m[0][0] * m[2][1] - m[2][0] * m[0][1])};
  inv[2] = vec<T, 3>{+(m[0][1] * m[1][2] - m[1][1] * m[0][2]),
                     -(m[0][0] * m[1][2] - m[1][0] * m[0][2]),
                     +(m[0][0] * m[1][1] - m[1][0] * m[0][1])};
  inv /= det;
  return inv;
}

template <FloatingPoint T>
constexpr mat<T, 4> Inverse(mat<T, 4> m) {
  T sub00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  T sub01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  T sub02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  T sub03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  T sub04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  T sub05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
  T sub06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
  T sub07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
  T sub08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
  T sub09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
  T sub10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
  T sub11 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
  T sub12 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
  T sub13 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
  T sub14 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
  T sub15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
  T sub16 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
  T sub17 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

  mat<T, 4> inv;
  inv[0] = vec<T, 4>{+(m[1][1] * sub00 - m[1][2] * sub01 + m[1][3] * sub02),
                     -(m[1][0] * sub00 - m[1][2] * sub03 + m[1][3] * sub04),
                     +(m[1][0] * sub01 - m[1][1] * sub03 + m[1][3] * sub05),
                     -(m[1][0] * sub02 - m[1][1] * sub04 + m[1][2] * sub05)};

  inv[1] = vec<T, 4>{-(m[0][1] * sub00 - m[0][2] * sub01 + m[0][3] * sub02),
                     +(m[0][0] * sub00 - m[0][2] * sub03 + m[0][3] * sub04),
                     -(m[0][0] * sub01 - m[0][1] * sub03 + m[0][3] * sub05),
                     +(m[0][0] * sub02 - m[0][1] * sub04 + m[0][2] * sub05)};

  inv[2] = vec<T, 4>{+(m[0][1] * sub06 - m[0][2] * sub07 + m[0][3] * sub08),
                     -(m[0][0] * sub06 - m[0][2] * sub09 + m[0][3] * sub10),
                     +(m[0][0] * sub07 - m[0][1] * sub09 + m[0][3] * sub11),
                     -(m[0][0] * sub08 - m[0][1] * sub10 + m[0][2] * sub11)};

  inv[3] = vec<T, 4>{-(m[0][1] * sub12 - m[0][2] * sub13 + m[0][3] * sub14),
                     +(m[0][0] * sub12 - m[0][2] * sub15 + m[0][3] * sub16),
                     -(m[0][0] * sub13 - m[0][1] * sub15 + m[0][3] * sub17),
                     +(m[0][0] * sub14 - m[0][1] * sub16 + m[0][2] * sub17)};

  T det = m[0][0] * inv[0][0] + m[0][1] * inv[0][1] + m[0][2] * inv[0][2] +
          m[0][3] * inv[0][3];

  assert(det != 0);

  inv /= det;
  return inv;
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