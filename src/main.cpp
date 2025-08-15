#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <print>
#include <thread>
#include <vector>

#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "render/color.h"
#include "window/window.h"

using namespace std::chrono_literals;

int32_t main([[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv) {
  softy::EventChannel channel{};
  softy::WindowDescriptor descriptor{
      .name = "softy",
      .width = 800,
      .height = 600,
  };
  softy::Window window{};
  std::vector<softy::Color> pixels(
      static_cast<std::size_t>(descriptor.width * descriptor.height));

  std::vector<softy::Color> test(
      static_cast<std::size_t>(descriptor.width * descriptor.height / 2),
      softy::Color{0xFF0000});
  std::copy(test.begin(), test.end(), pixels.begin());

  softy::v4f v0{0.0f, 1.0f, 2.0f, 3.0f};
  softy::v4f v1{0.0f, 1.0f, 2.0f};
  softy::v4f v2{v1};
  softy::v4f v4 = softy::v4f::Zero();
  softy::v4f v5 = softy::v4f::One();
  softy::v4f v6 = softy::v4f::Basis(1);
  softy::v2f v7{0.0f};
  softy::v3f v8{0.0f, 1.0f};
  softy::v2i v9{0};
  softy::v3i v10{0, 1};
  softy::v4i v11{0, 1, 2};
  softy::mat4 m0{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  softy::mat4 m1{m0};
  softy::mat4 m2{softy::v2f{0, 1}, softy::v2f{2, 3}};

  std::println("{}", softy::equals(v0, v0));
  std::println("{}", v1);
  std::println("{}", v2);
  std::println("{}", v4);
  std::println("{}", v5);
  std::println("{}", v6);
  std::println("{}", v7);
  std::println("{}", v8);
  std::println("{}", v9);
  std::println("{}", v10);
  std::println("{}\n", v11);
  std::println("{}\n", m0);
  std::println("{}\n", softy::Transpose(m0));
  std::println("{}\n", m0 + m1);
  std::println("{}\n", m0 - m1);
  std::println("{}\n", m0 * m1);
  std::println("{}\n", m2);

  if (!window.Create(descriptor, channel, pixels)) {
    return EXIT_FAILURE;
  }

  while (true) {
    auto start = std::chrono::high_resolution_clock::now();

    if (!window.Update()) {
      break;
    }
    window.Present();

    auto end = std::chrono::high_resolution_clock::now();
    auto diff = (end - start).count() / 1e9;
    window.SetTitle(std::format("{} {:.2f}", descriptor.name, 1 / diff));
  }

  return EXIT_SUCCESS;
}