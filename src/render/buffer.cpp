#include "render/buffer.h"

#include <stdlib.h>

#include <any>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>

#include "math/math.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "render/color.h"
#include "render/vertex.h"

namespace softy {
bool CohenSutherlandClip(v2i& v0, v2i& v1, v2i min, v2i max) {
  auto GetRegion = [](v2i v, v2i min, v2i max) {
    int32_t region{0};
    if (v[0] < min[0]) {
      region |= 0b0001;
    }
    if (v[0] > max[0]) {
      region |= 0b0010;
    }
    if (v[1] < min[1]) {
      region |= 0b0100;
    }
    if (v[1] > max[1]) {
      region |= 0b1000;
    }
    return region;
  };

  while (true) {
    auto r0 = GetRegion(v0, min, max);
    auto r1 = GetRegion(v1, min, max);

    if (r0 & r1) {
      return false;
    }

    if (!(r0 | r1)) {
      break;
    }

    float slope =
        static_cast<float>(v1[1] - v0[1]) / static_cast<float>(v1[0] - v0[0]);

    auto Clip = [&](v2i& v, int32_t r) {
      float t = numbers::fRad2Deg * acos(dot(normalize(v), v2f::Basis(0)));
      if (r & 0b0001) {
        v[1] = static_cast<int>(slope * static_cast<float>(min[0] - v[0]) +
                                static_cast<float>(v[1]));
        v[0] = min[0];
      } else if (r & 0b0010) {
        v[1] = static_cast<int>(slope * static_cast<float>(max[0] - v[0]) +
                                static_cast<float>(v[1]));
        v[0] = max[0];
      } else if (r & 0b0100) {
        v[0] = static_cast<int>(static_cast<float>(min[1] - v[1]) / slope +
                                static_cast<float>(v[0]));
        v[1] = min[1];
      } else if (r & 0b1000) {
        v[0] = static_cast<int>(static_cast<float>(max[1] - v[1]) / slope +
                                static_cast<float>(v[0]));
        v[1] = max[1];
      }
    };

    Clip(v0, r0);
    Clip(v1, r1);
  }

  return true;
}

Buffer::Buffer(std::size_t bits, std::size_t size) { Allocate(bits, size); }

Buffer::~Buffer() {
  if (data_ != nullptr) {
    free(data_);
    data_ = nullptr;
    size_ = 0uz;
    capacity_ = 0uz;
  }
}

Buffer::Buffer(const Buffer& buffer) {
  assert(buffer.data_ != nullptr);
  data_ = malloc(buffer.capacity_);
  if (data_) {
    size_ = buffer.size_;
    capacity_ = buffer.capacity_;
    memcpy(data_, buffer.data_, buffer.capacity_);
  }
}

Buffer& Buffer::operator=(const Buffer& buffer) {
  assert(buffer.data_ != nullptr);
  data_ = malloc(buffer.capacity_);
  if (data_) {
    size_ = buffer.size_;
    capacity_ = buffer.capacity_;
    memcpy(data_, buffer.data_, buffer.capacity_);
  }
  return *this;
}

Buffer::Buffer(Buffer&& buffer) {
  data_ = buffer.data_;
  size_ = buffer.size_;
  capacity_ = buffer.capacity_;

  buffer.data_ = nullptr;
  buffer.size_ = 0uz;
  buffer.capacity_ = 0uz;
}

Buffer& Buffer::operator=(Buffer&& buffer) {
  data_ = buffer.data_;
  size_ = buffer.size_;
  capacity_ = buffer.capacity_;

  buffer.data_ = nullptr;
  buffer.size_ = 0uz;
  buffer.capacity_ = 0uz;

  return *this;
}

void Buffer::Allocate(std::size_t bits, std::size_t size) {
  assert(bits > 0 && size > 0);
  std::size_t bytes = (bits * size + 7uz) / 8uz;
  data_ = malloc(bytes);
  if (data_) {
    size_ = size;
    capacity_ = bytes;
  }
}

ColorBuffer::ColorBuffer(int32_t width, int32_t height)
    : buffer_{BitCount<Color>(), static_cast<std::size_t>(width * height)},
      width_{width},
      height_{height} {}

void ColorBuffer::SetSize(int32_t width, int32_t height) noexcept {
  buffer_.Allocate(BitCount<Color>(), static_cast<std::size_t>(width * height));
  width_ = width;
  height_ = height;
}

Color ColorBuffer::GetPixel(int32_t x, int32_t y) {
  assert(x > 0 && x <= width_ && y > 0 && y < height_);
  Color* colors = buffer_.Get<Color>();
  return colors[y * width_ + x];
}

void ColorBuffer::SetPixel(int32_t x, int32_t y, Color color) {
  assert(x >= 0 && x < width_ && y >= 0 && y < height_);
  Color* colors = buffer_.Get<Color>();
  colors[y * width_ + x] = color;
}

void ColorBuffer::DrawLine(v2i v0, v2i v1, Color color) {
  if (!CohenSutherlandClip(v0, v1, v2i::Zero(), v2i{width_ - 1, height_ - 1})) {
    return;
  }

  assert(v0[0] >= 0 && v0[0] < width_ && v0[1] >= 0 && v0[1] < height_ &&
         v1[0] >= 0 && v1[0] < width_ && v1[1] >= 0 && v1[1] < height_);
  // Line drawing algorithm from
  // https://zingl.github.io/Bresenham.pdf
  int32_t xi = v0[0] < v1[0] ? 1 : -1;
  int32_t yi = v0[1] < v1[1] ? 1 : -1;
  int32_t dx = static_cast<int32_t>(abs(v1[0] - v0[0]));
  int32_t dy = static_cast<int32_t>(-abs(v1[1] - v0[1]));
  int32_t e = dx + dy;
  v2i cur = v0;
  v2i end = v1;

  while (true) {
    SetPixel(cur[0], cur[1], color);

    int32_t e2 = 2 * e;
    if (e2 >= dy) {
      if (cur[0] == end[0]) break;
      e += dy;
      cur[0] += xi;
    }

    if (e2 <= dx) {
      if (cur[1] == end[1]) break;
      e += dx;
      cur[1] += yi;
    }
  }
}

void ColorBuffer::Clear(Color color) {
  Color* pixels = reinterpret_cast<Color*>(buffer_.Get());
  std::size_t n = static_cast<std::size_t>(GetWidth() * GetHeight());

  for (int i = 0; i < n; ++i) {
    pixels[i] = color;
  }
}

DepthBuffer::DepthBuffer(int32_t width, int32_t height)
    : buffer_{BitCount<float>(), static_cast<std::size_t>(width * height)},
      width_{width},
      height_{height} {}

void DepthBuffer::SetSize(int32_t width, int32_t height) noexcept {
  buffer_.Allocate(BitCount<float>(), static_cast<std::size_t>(width * height));
  width_ = width;
  height_ = height;
}

float DepthBuffer::GetDepth(int32_t x, int32_t y) {
  assert(x >= 0 && x < width_ && y > 0 && y < height_);
  float* depths = buffer_.Get<float>();
  return depths[y * width_ + x];
}

void DepthBuffer::SetDepth(int32_t x, int32_t y, float depth) {
  assert(x >= 0 && x < width_ && y >= 0 && y < height_);
  float* depths = buffer_.Get<float>();
  depths[y * width_ + x] = depth;
}

ConstantBuffer::ConstantBuffer()
    : buffer_(BitCount<ConstantBufferData>(), 1uz) {}

ConstantBuffer::ConstantBuffer(ConstantBufferData data) : ConstantBuffer() {
  SetData(data);
}

void ConstantBuffer::SetData(ConstantBufferData data) noexcept {
  ConstantBufferData* cbd = Get();
  *cbd = data;
}

void ConstantBuffer::SetWorldMatrix(mat4 matWorld) noexcept {
  ConstantBufferData* cbd = Get();
  cbd->matWorld = matWorld;
}

void ConstantBuffer::SetViewMatrix(mat4 matView) noexcept {
  ConstantBufferData* cbd = Get();
  cbd->matView = matView;
}

void ConstantBuffer::SetProjectionMatrix(mat4 matProjection) noexcept {
  ConstantBufferData* cbd = Get();
  cbd->matProjection = matProjection;
}

void ConstantBuffer::SetProperties(
    const std::unordered_map<std::string, std::any>* properties) {
  ConstantBufferData* cbd = Get();
  cbd->properties = properties;
}

mat4 ConstantBuffer::GetWorldMatrix() const noexcept {
  const ConstantBufferData* cbd = Get();
  return cbd->matWorld;
}

mat4 ConstantBuffer::GetViewMatrix() const noexcept {
  const ConstantBufferData* cbd = Get();
  return cbd->matView;
}

mat4 ConstantBuffer::GetProjectionMatrix() const noexcept {
  const ConstantBufferData* cbd = Get();
  return cbd->matProjection;
}

const std::unordered_map<std::string, std::any>* ConstantBuffer::GetProperties()
    const noexcept {
  const ConstantBufferData* cbd = Get();
  return cbd->properties;
}

ConstantBufferData* ConstantBuffer::Get() noexcept {
  return reinterpret_cast<ConstantBufferData*>(buffer_.Get());
}

const ConstantBufferData* ConstantBuffer::Get() const noexcept {
  return reinterpret_cast<const ConstantBufferData*>(buffer_.Get());
}
}  // namespace softy