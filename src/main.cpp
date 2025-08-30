#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <format>
#include <print>
#include <thread>

#include "core/property.h"
#include "core/transform.h"
#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "render/buffer.h"
#include "render/camera.h"
#include "render/color.h"
#include "render/forward_render_pipeline.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/render_pipeline.h"
#include "shader/shader.h"
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

  softy::ConstantBuffer cb{};
  softy::ColorBuffer rt{descriptor.width, descriptor.height};
  softy::DepthBuffer db{descriptor.width, descriptor.height};
  std::unique_ptr<softy::RenderPipeline> renderPipeline(
      new softy::ForwardRenderPipeline());

  renderPipeline->SetConstantBuffer(&cb);

  if (!window.Create(descriptor, channel, rt.GetData())) {
    return EXIT_FAILURE;
  }

  softy::Shader unlitColorShader{softy::UnlitColorShader()};
  softy::Color color{0xFFFF0000};
  softy::Material material{&unlitColorShader};
  material.SetProperty("Color_", color);

  softy::Mesh cube{std::array{
                       softy::Vertex(softy::v4f{-0.5f, -0.5f, -0.5f, 1.0f}),
                       softy::Vertex(softy::v4f{-0.5f, -0.5f, +0.5f, 1.0f}),
                       softy::Vertex(softy::v4f{+0.5f, -0.5f, +0.5f, 1.0f}),
                       softy::Vertex(softy::v4f{+0.5f, -0.5f, -0.5f, 1.0f}),
                       softy::Vertex(softy::v4f{-0.5f, +0.5f, -0.5f, 1.0f}),
                       softy::Vertex(softy::v4f{-0.5f, +0.5f, +0.5f, 1.0f}),
                       softy::Vertex(softy::v4f{+0.5f, +0.5f, +0.5f, 1.0f}),
                       softy::Vertex(softy::v4f{+0.5f, +0.5f, -0.5f, 1.0f}),
                   },
                   std::array<std::size_t, 36>{
                       0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 1, 5, 4, 1, 4, 0,
                       2, 6, 5, 2, 5, 1, 3, 7, 6, 3, 6, 2, 4, 5, 6, 4, 6, 7},
                   &material};
  softy::Transform cubeTransform{};

  softy::Camera cam{&rt};

  auto start = std::chrono::high_resolution_clock::now();
  while (true) {
    auto end = std::chrono::high_resolution_clock::now();
    auto dt = (end - start).count() / 1e9;
    window.SetTitle(std::format("{} {:.2f}", descriptor.name, 1 / dt));

    start = end;

    if (!window.Update()) {
      break;
    }

    cam.GetTransform().position = softy::v3f{0.0f, 0.0f, -2.5f};
    // cam.GetTransform().rotation += softy::v3f(0.0f, 60.0f * dt, 0.0f);
    cam.GetTransform().LookAt(softy::v3f{0.0f, 0.0f, 0.0f}, true);
    // cubeTransform.scale = softy::v3f::One() * 2.0f;
    cubeTransform.rotation += softy::v3f(0.0f, 60.0f * dt, 0.0f);

    cb.SetData(softy::ConstantBufferData{
        .matWorld = softy::mat4::Identity(),
        .matView = cam.GetViewMatrix(),
        .matProjection = cam.GetProjectionMatrix(),
    });

    renderPipeline->AddObject(&cube, cubeTransform.GetTRS());
    renderPipeline->Render(&cam);
    window.Present();

    rt.Clear(softy::Color::Black());
  }

  return EXIT_SUCCESS;
}