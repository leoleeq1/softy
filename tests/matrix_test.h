#ifndef MATRIX_TEST_H_
#define MATRIX_TEST_H_

#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "unit_test.h"

TEST(Matrix, TestVectorConstructur) {
  constexpr softy::v4f v1{softy::v4f::One()};
  constexpr softy::mat4 actual{v1, v1, v1, v1};
  constexpr softy::mat4 expected{
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  };
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Matrix, TestIdentity) {
  constexpr softy::mat4 expected{
      1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  };
  constexpr auto identity = softy::mat4::Identity();
  ASSERT_EQ_FLOAT(expected, identity);
}

TEST(Matrix, TestTranspose) {
  constexpr softy::mat2 m{1.0f, 2.0f, 3.0f, 4.0f};
  constexpr softy::mat2 expected{1.0f, 3.0f, 2.0f, 4.0f};
  const auto transpose = softy::Transpose(m);
  ASSERT_EQ_FLOAT(expected, transpose);
}

TEST(Matrix, TestMatrixAdd) {
  constexpr softy::mat4 m{
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  };
  constexpr softy::mat4 expected = m * 2.0f;
  constexpr auto actual = m + m;
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Matrix, TestMatrixSubtract) {
  constexpr softy::mat4 m{
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  };
  constexpr softy::mat4 expected = m * 0.0f;
  constexpr auto actual = m - m;
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Matrix, TestMatrixMultiply) {
  constexpr softy::mat2 m0{1.0f, 2.0f, 3.0f, 4.0f};
  constexpr softy::mat2 m1{-5.0f, 6.0f, 7.0f, -8.0f};
  constexpr softy::mat2 expected{9.0f, -10.0f, 13.0f, -14.0f};
  constexpr auto actual = m0 * m1;
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Matrix, TestMatrixVectorMultiply) {
  constexpr softy::v4f v{0.0f, 1.0f, 2.0f, 3.0f};
  constexpr softy::mat4 m{
      1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f,
      1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f,
  };
  constexpr softy::v4f expected{6.0f, 12.0f, 18.0f, 24.0f};
  constexpr auto actual = v * m;
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Matrix, TestMatrixScalarDivision) {
  constexpr softy::mat4 m{
      2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f,
      2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f,
  };
  constexpr softy::mat4 expected{
      0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
      0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
  };
  constexpr auto actual = 1.0f / m;
  ASSERT_EQ_FLOAT(expected, actual);
}

#endif  // MATRIX_TEST_H_