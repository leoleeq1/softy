#ifndef TYPE_TYPE_ID_H_
#define TYPE_TYPE_ID_H_

#include <cstdint>
#include <string_view>

namespace softy {
constexpr uint32_t fnv1a_hash(std::string_view sv) {
  uint32_t hash = 2166136261U;
  for (char c : sv) {
    hash ^= static_cast<uint32_t>(c);
    hash *= 16777619U;
  }
  return hash;
}

template <typename T>
struct type_id {
  static consteval uint32_t get() {
#if defined(__clang__) || defined(__GNUC__)
    return fnv1a_hash(__PRETTY_FUNCTION__);
#elif defined(_MSC_VER)
    return fnv1a_hash(__FUNCSIG__);
#else
    static_assert(false, "Not supported compiler!");
    return 0;
#endif
  }

  static constexpr uint32_t value = get();
};
}  // namespace softy

#endif  // TYPE_TYPE_ID_H_