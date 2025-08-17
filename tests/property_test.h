#ifndef PROPERTY_TEST_H_
#define PROPERTY_TEST_H_

#include <cstdint>

#include "core/property.h"
#include "unit_test.h"

TEST(Property, TestGetter) {
  softy::Property<int32_t> p{0, [](auto& v) { return v + 1; }, nullptr};
  int32_t expected = 1;
  int32_t actual = p;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestSetter) {
  int32_t expected = 2;
  int32_t actual = 0;
  softy::Property<int32_t> p{0, nullptr, [&](auto& v) { return ++actual; }};
  p = 0;
  p = 1;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestAdd) {
  softy::Property<int32_t> p{0};
  int32_t q = 42;
  int32_t expected = 42;
  int32_t actual = p + q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestSubtract) {
  softy::Property<int32_t> p{0};
  int32_t q = 42;
  int32_t expected = -42;
  int32_t actual = p - q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestMultiply) {
  softy::Property<int32_t> p{0};
  int32_t q = 42;
  int32_t expected = 0;
  int32_t actual = p * q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestDivide) {
  softy::Property<float> p{1.0f};
  float q = 2.0f;
  float expected = 0.5f;
  float actual = p / q;
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Property, TestModulo) {
  softy::Property<int32_t> p{45};
  int32_t q = 42;
  int32_t expected = 3;
  int32_t actual = p % q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestIncrementalPrefix) {
  softy::Property<int32_t> p{41};
  int32_t expected = 42;
  int32_t actual = ++p;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestIncrementalPostfix) {
  softy::Property<int32_t> p{42};
  int32_t expected = 42;
  int32_t actual = p++;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestDecrementalPrefix) {
  softy::Property<int32_t> p{43};
  int32_t expected = 42;
  int32_t actual = --p;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestDecrementalPostfix) {
  softy::Property<int32_t> p{42};
  int32_t expected = 42;
  int32_t actual = p--;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestGreaterThanEqual) {
  softy::Property<int32_t> p{45};
  int32_t q = 42;
  bool expected = true;
  bool actual = p >= q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestGreaterThanEqual2) {
  softy::Property<int32_t> p{45};
  softy::Property<int32_t> q{42};
  bool expected = true;
  bool actual = p >= q;
  ASSERT_EQ(expected, actual);
}

#endif  // PROPERTY_TEST_H_