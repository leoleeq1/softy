#ifndef VECTOR_TEST_H_
#define VECTOR_TEST_H_

#include "math/math.h"
#include "math/vector.h"
#include "unit_test.h"

TEST(Vector, TestIntegralVectorNotEqual) {
  constexpr softy::v2i v0{0, 1};
  constexpr softy::v2i v1{1, 0};
  constexpr auto actual = v0 != v1;
  ASSERT_EQ(true, actual);
}

TEST(Vector, TestOne) {
  constexpr softy::v4f v0{1.0f, 1.0f, 1.0f, 1.0f};
  constexpr auto actual = softy::v4f::One();
  ASSERT_EQ_FLOAT(v0, actual);
}

TEST(Vector, TestZero) {
  constexpr softy::v4f v0{0.0f, 0.0f, 0.0f, 0.0f};
  constexpr auto actual = softy::v4f::Zero();
  ASSERT_EQ_FLOAT(v0, actual);
}

TEST(Vector, TestBasis) {
  constexpr softy::v4f v0{0.0f, 1.0f, 0.0f, 0.0f};
  constexpr auto actual = softy::v4f::Basis(1);
  ASSERT_EQ_FLOAT(v0, actual);
}

TEST(Vector, TestAdd) {
  constexpr softy::v3f v0f{0.0f, 1.0f, 2.0f};
  constexpr softy::v3f v1f{3.0f, 4.0f, 5.0f};
  constexpr softy::v3f expected{3.0f, 5.0f, 7.0f};
  constexpr auto actual = (v0f + v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestSubtract) {
  constexpr softy::v3f v0f{0.0f, 1.0f, 2.0f};
  constexpr softy::v3f v1f{3.0f, 4.0f, 5.0f};
  constexpr softy::v3f expected{-3.0f, -3.0f, -3.0f};
  constexpr auto actual = (v0f - v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestMultiply) {
  constexpr softy::v3f v0f{0.0f, 1.0f, 2.0f};
  constexpr softy::v3f v1f{3.0f, 4.0f, 5.0f};
  constexpr softy::v3f expected{0.0f, 4.0f, 10.0f};
  constexpr auto actual = (v0f * v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestDivision) {
  constexpr softy::v3f v0f{0.0f, 1.0f, 2.0f};
  constexpr softy::v3f v1f{3.0f, 4.0f, 5.0f};
  constexpr softy::v3f expected{0.0f, 0.25f, 0.4f};
  constexpr auto actual = (v0f / v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestAdd2) {
  constexpr softy::v4f v0f{0.0f, 1.0f, 2.0f};
  constexpr softy::v4f v1f{3.0f, 4.0f, 5.0f};
  constexpr softy::v4f expected{3.0f, 5.0f, 7.0f};
  constexpr auto actual = (v0f + v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestSubtract2) {
  constexpr softy::v4f v0f{0.0f, 1.0f, 2.0f};
  constexpr softy::v4f v1f{3.0f, 4.0f, 5.0f};
  constexpr softy::v4f expected{-3.0f, -3.0f, -3.0f};
  constexpr auto actual = (v0f - v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestMultiply2) {
  constexpr softy::v4f v0f{0.0f, 1.0f, 2.0f};
  constexpr softy::v4f v1f{3.0f, 4.0f, 5.0f};
  constexpr softy::v4f expected{0.0f, 4.0f, 10.0f};
  constexpr auto actual = (v0f * v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestDivision2) {
  constexpr softy::v4f v0f{0.0f, 1.0f, 2.0f, 1.0f};
  constexpr softy::v4f v1f{3.0f, 4.0f, 5.0f, 0.5f};
  constexpr softy::v4f expected{0.0f, 0.25f, 0.4f, 2.0f};
  const auto actual = v0f / v1f;
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestCross) {
  constexpr softy::v3f v0f{softy::v3f::Basis(0)};
  constexpr softy::v3f v1f{softy::v3f::Basis(1)};
  constexpr softy::v3f expected{softy::v3f::Basis(2)};
  constexpr auto actual = softy::cross(v0f, v1f);
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestLength) {
  const auto length = softy::length(softy::v3f(0.0f, 1.0f, 2.0f));
  const auto expected = std::sqrt(5.0f);
  ASSERT_EQ_FLOAT(expected, length);
}

TEST(Vector, TestSqrLength) {
  const auto sqrLength = softy::sqrLength(softy::v3f(0.0f, 1.0f, 2.0f));
  const auto expected = 5.0f;
  ASSERT_EQ_FLOAT(expected, sqrLength);
}

TEST(Vector, TestNormalize) {
  constexpr softy::v4f expected{0.5f, 0.5f, 0.5f, 0.5f};
  const auto actual = softy::normalize(softy::v4f(1.0f, 1.0f, 1.0f, 1.0f));
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Vector, TestReflect) {
  constexpr softy::v3f n{softy::v3f::Basis(1)};
  constexpr softy::v3f p{softy::v3f{1.0f, 1.0f, 0.0f}};
  constexpr softy::v3f expected{-1.0f, 1.0f, 0.0f};
  constexpr softy::v3f actual{softy::reflect(p, n)};
  ASSERT_EQ_FLOAT(expected, actual);
}

#endif  // VECTOR_TEST_H_