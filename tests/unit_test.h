#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include <cstdint>
#include <cstdio>
#include <functional>
#include <print>
#include <string>
#include <vector>

#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"

inline std::vector<std::function<void()>> test_functions;
inline constexpr const char* formatString =
    "\nAssertion failed: \n {}: {}\n {}: {}\n  at {}:{}";

#define TEST(TestSuiteName, TestCaseName)                       \
  void TestSuiteName##_##TestCaseName();                        \
  struct RegisterTest_##TestSuiteName##_##TestCaseName {        \
    RegisterTest_##TestSuiteName##_##TestCaseName() {           \
      test_functions.push_back(TestSuiteName##_##TestCaseName); \
    }                                                           \
  } register_test_##TestSuiteName##_##TestCaseName;             \
  void TestSuiteName##_##TestCaseName()

#define ASSERT_EQ(expected, actual)                                    \
  if ((expected) != (actual)) {                                        \
    std::println(stderr, formatString, #expected, (expected), #actual, \
                 (actual), __FILE__, __LINE__);                        \
    throw "Assertion failed";                                          \
  }

#define ASSERT_EQ_FLOAT(expected, actual)                              \
  if (!softy::equals((expected), (actual))) {                          \
    std::println(stderr, formatString, #expected, (expected), #actual, \
                 (actual), __FILE__, __LINE__);                        \
    throw "Assertion failed";                                          \
  }

inline void run_all_tests() {
  int32_t passed = 0;
  int32_t failed = 0;
  for (const auto& testFunc : test_functions) {
    try {
      testFunc();
      ++passed;
    } catch (const char* msg) {
      std::println(stderr, "Test failed: {}", msg);
      ++failed;
    } catch (...) {
      std::println(stderr, "Unknown exception during test.");
      ++failed;
    }
  }

  std::println("\n--- Test Summary ---");
  std::println("Passed: {}", passed);
  std::println("Failed: {}", failed);
}

#endif  // UNIT_TEST_H_