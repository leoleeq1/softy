#include "render/rasterizer.h"

#include <functional>
#include <limits>

#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "render/buffer.h"
#include "render/color.h"
#include "render/material.h"
#include "render/vertex.h"
#include "shader/shader.h"

namespace softy {
static bool BackfaceCulling(v2f v0, v2f v1, v2f v2) {
  return cross(v1 - v0, v2 - v0) < 0.0f;
}

static bool FrustumCulling(v4f v0, v4f v1, v4f v2) {
  if (abs(v0[0]) > v0[3] && abs(v1[0]) > v1[3] && abs(v2[0]) > v2[3]) {
    return true;
  }

  if (abs(v0[1]) > v0[3] && abs(v1[1]) > v1[3] && abs(v2[1]) > v2[3]) {
    return true;
  }

  if (v0[2] > v0[3] && v1[2] > v1[3] && v2[2] > v2[3]) {
    return true;
  }

  return false;
}

enum class HomogeneousPlane {
  PositiveW,
  PositiveX,
  NegativeX,
  PositiveY,
  NegativeY,
  PositiveZ,
  NegativeZ,
};

static int32_t IsVisible(v4f v) {
  return abs(v[0]) <= v[3] && abs(v[1]) <= v[3] && abs(v[2]) <= v[3];
}

static int32_t IsInisdePlane(v4f v, HomogeneousPlane plane) {
  switch (plane) {
    case HomogeneousPlane::PositiveW:
      return v[3] >= std::numeric_limits<float>::epsilon();
    case HomogeneousPlane::PositiveX:
      return v[0] <= v[3];
    case HomogeneousPlane::NegativeX:
      return v[0] >= -v[3];
    case HomogeneousPlane::PositiveY:
      return v[1] <= v[3];
    case HomogeneousPlane::NegativeY:
      return v[1] >= -v[3];
    case HomogeneousPlane::PositiveZ:
      return v[2] <= v[3];
    case HomogeneousPlane::NegativeZ:
      return v[2] >= -v[3];
    default:
      assert(false);
      return 0;
  }
}

static float fraction(v4f prev, v4f cur, HomogeneousPlane plane) {
  switch (plane) {
    case HomogeneousPlane::PositiveW:
      return (prev[3] - std::numeric_limits<float>::epsilon()) /
             (prev[3] - cur[3]);
    case HomogeneousPlane::PositiveX:
      return (prev[3] - prev[0]) / ((prev[3] - prev[0]) - (cur[3] - cur[0]));
    case HomogeneousPlane::NegativeX:
      return (prev[3] + prev[0]) / ((prev[3] + prev[0]) - (cur[3] + cur[0]));
    case HomogeneousPlane::PositiveY:
      return (prev[3] - prev[1]) / ((prev[3] - prev[1]) - (cur[3] - cur[1]));
    case HomogeneousPlane::NegativeY:
      return (prev[3] + prev[1]) / ((prev[3] + prev[1]) - (cur[3] + cur[1]));
    case HomogeneousPlane::PositiveZ:
      return (prev[3] - prev[2]) / ((prev[3] - prev[2]) - (cur[3] - cur[2]));
    case HomogeneousPlane::NegativeZ:
      return (prev[3] + prev[2]) / ((prev[3] + prev[2]) - (cur[3] + cur[2]));
    default:
      assert(false);
      return 0;
  }
}

static std::vector<VertexOutput> PlaneClipping(
    HomogeneousPlane plane, const std::vector<VertexOutput>& inputs) {
  if (inputs.size() < 3) {
    return {};
  }

  std::vector<VertexOutput> outputs;

  for (std::size_t i = 0; i < inputs.size(); ++i) {
    const VertexOutput& prev = inputs[(inputs.size() + i - 1) % inputs.size()];
    int32_t isPrevInside = IsInisdePlane(prev.position, plane);
    const VertexOutput& cur = inputs[i];
    int32_t isCurInside = IsInisdePlane(cur.position, plane);

    if (isPrevInside != isCurInside) {
      float f = fraction(prev.position, cur.position, plane);
      VertexOutput out = lerp(prev, cur, f);
      outputs.push_back(out);
    }

    if (isCurInside) {
      outputs.push_back(cur);
    }
  }

  if (outputs.size() < 3) {
    return {};
  }

  return outputs;
}

std::vector<VertexOutput> HomogeneousClipping(
    std::vector<VertexOutput>& inputs) {
  std::vector<VertexOutput> outputs;

  std::vector<VertexOutput> verts;
  for (std::size_t i = 0; i < inputs.size(); i += 3) {
    VertexOutput& v0 = inputs[i + 0];
    VertexOutput& v1 = inputs[i + 1];
    VertexOutput& v2 = inputs[i + 2];

    if (IsVisible(v0.position) && IsVisible(v1.position) &&
        IsVisible(v2.position)) {
      outputs.push_back(v0);
      outputs.push_back(v1);
      outputs.push_back(v2);
      continue;
    }

    verts.push_back(v0);
    verts.push_back(v1);
    verts.push_back(v2);

    for (int j = 0; j < 7; ++j) {
      verts = PlaneClipping(static_cast<HomogeneousPlane>(j), verts);
    }

    for (std::size_t j = 2; j < verts.size(); ++j) {
      VertexOutput& out0 = verts[0];
      VertexOutput& out1 = verts[j - 1];
      VertexOutput& out2 = verts[j - 0];

      outputs.push_back(out0);
      outputs.push_back(out1);
      outputs.push_back(out2);
    }

    verts.clear();
  }

  return outputs;
}

void Rasterize(ColorBuffer& renderTarget,
               const std::vector<VertexOutput>& vsOutputs,
               const std::vector<int>& indices, FragmentShader fs) {
  std::vector<VertexOutput> culled;

  for (std::size_t i = 0; i < indices.size(); i += 3) {
    const VertexOutput& v0 =
        vsOutputs[static_cast<std::size_t>(indices[i + 0])];
    const VertexOutput& v1 =
        vsOutputs[static_cast<std::size_t>(indices[i + 1])];
    const VertexOutput& v2 =
        vsOutputs[static_cast<std::size_t>(indices[i + 2])];

    if (FrustumCulling(v0.position, v1.position, v2.position)) {
      continue;
    }

    culled.push_back(v0);
    culled.push_back(v1);
    culled.push_back(v2);
  }

  culled = HomogeneousClipping(culled);

  float halfWidth = static_cast<float>(renderTarget.GetWidth() / 2);
  float halfHeight = static_cast<float>(renderTarget.GetHeight() / 2);

  for (std::size_t i = 0; i < culled.size(); i += 3) {
    for (std::size_t j = 0; j < 3; ++j) {
      culled[i + j].position =
          culled[i + j].position / culled[i + j].position[3];
      culled[i + j].position[0] =
          culled[i + j].position[0] * halfWidth + halfWidth;
      culled[i + j].position[1] =
          culled[i + j].position[1] * halfHeight + halfHeight;
    }

    VertexOutput& v0 = culled[i + 0];
    VertexOutput& v1 = culled[i + 1];
    VertexOutput& v2 = culled[i + 2];

    if (BackfaceCulling(v0.position, v1.position, v2.position)) {
      continue;
    }

    renderTarget.DrawLine(v0.position, v1.position, Color::Red());
    renderTarget.DrawLine(v1.position, v2.position, Color::Red());
    renderTarget.DrawLine(v0.position, v2.position, Color::Red());
  }
}
}  // namespace softy