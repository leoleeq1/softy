#ifndef PROPERTY_TEST_H_
#define PROPERTY_TEST_H_

#include <cstdint>

#include "core/property.h"
#include "unit_test.h"

TEST(Property, TestCopyConstructor) {
  int32_t p = 1;
  softy::Property<int32_t> prop{[&]() { return p; }, [&](int32_t v) { p = v; }};
  int32_t expected = 1;
  int32_t actual = prop;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestGetter) {
  int32_t p = 1;
  softy::Property<int32_t> prop{[&]() { return p; }};
  int32_t expected = 1;
  int32_t actual = prop;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestSetter) {
  int32_t expected = 2;
  int32_t actual = 0;
  softy::Property<int32_t> prop{nullptr, [&](auto& v) { return actual = v; }};
  prop = 2;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestAdd) {
  int32_t p{0};
  softy::Property<int32_t> prop{[&]() { return p; }, [&](int32_t v) { p = v; }};
  int32_t q = 42;
  int32_t expected = 42;
  int32_t actual = prop + q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestSubtract) {
  int32_t p{0};
  softy::Property<int32_t> prop{[&]() { return p; }, [&](int32_t v) { p = v; }};
  int32_t q = 42;
  int32_t expected = -42;
  int32_t actual = prop - q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestMultiply) {
  int32_t p{0};
  softy::Property<int32_t> prop{[&]() { return p; }, [&](int32_t v) { p = v; }};
  int32_t q = 42;
  int32_t expected = 0;
  int32_t actual = prop * q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestDivide) {
  float p{1.0f};
  softy::Property<float> prop{[&]() { return p; }, [&](float v) { p = v; }};
  float q = 2.0f;
  float expected = 0.5f;
  float actual = prop / q;
  ASSERT_EQ_FLOAT(expected, actual);
}

TEST(Property, TestModulo) {
  int32_t p{45};
  softy::Property<int32_t> prop{[&]() { return p; }, [&](int32_t v) { p = v; }};
  int32_t q = 42;
  int32_t expected = 3;
  int32_t actual = prop % q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestGreaterThanEqual) {
  int32_t p{45};
  softy::Property<int32_t> prop{[&]() { return p; }, [&](int32_t v) { p = v; }};
  int32_t q = 42;
  bool expected = true;
  bool actual = prop >= q;
  ASSERT_EQ(expected, actual);
}

TEST(Property, TestGreaterThanEqual2) {
  int32_t p{45};
  softy::Property<int32_t> pProp{[&]() { return p; },
                                 [&](int32_t v) { p = v; }};
  int32_t q{42};
  softy::Property<int32_t> qProp{[&]() { return q; },
                                 [&](int32_t v) { q = v; }};
  bool expected = true;
  bool actual = pProp >= qProp;
  ASSERT_EQ(expected, actual);
}

#endif  // PROPERTY_TEST_H_