#ifndef RENDER_BUFFER_H_
#define RENDER_BUFFER_H_

#include <any>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

#include "math/matrix.h"
#include "math/vector.h"
#include "render/color.h"
#include "render/vertex.h"

namespace softy {
class Buffer {
 public:
  Buffer() = default;
  Buffer(std::size_t bits, std::size_t size);
  ~Buffer();
  Buffer(const Buffer& buffer);
  Buffer& operator=(const Buffer& buffer);
  Buffer(Buffer&& buffer);
  Buffer& operator=(Buffer&& buffer);

  void Allocate(std::size_t bits, std::size_t size);

  const void* Get() const noexcept { return data_; }
  void* Get() noexcept { return data_; }
  template <typename T>
  T* Get() noexcept {
    return reinterpret_cast<T*>(data_);
  }

  std::size_t GetCapacity() const noexcept { return capacity_; }

 private:
  void* data_ = nullptr;
  std::size_t bits_{};
  std::size_t size_{};
  std::size_t capacity_{};
};

class ColorBuffer {
 public:
  ColorBuffer() = default;
  ColorBuffer(int32_t width, int32_t height);
  ~ColorBuffer() = default;

  void SetSize(int32_t width, int32_t height) noexcept;

  int32_t GetWidth() const noexcept { return width_; }
  int32_t GetHeight() const noexcept { return height_; }
  int32_t GetSize() const noexcept { return width_ * height_; }
  Buffer& GetData() noexcept { return buffer_; }

  Color GetPixel(int32_t x, int32_t y);
  void SetPixel(int32_t x, int32_t y, Color color);
  void DrawLine(v2i v0, v2i v1, Color color);
  void Clear(Color color);

 private:
  Buffer buffer_;
  int32_t width_{};
  int32_t height_{};
};

class DepthBuffer {
 public:
  DepthBuffer() = default;
  DepthBuffer(int32_t width, int32_t height);
  ~DepthBuffer() = default;

  void SetSize(int32_t width, int32_t height) noexcept;

  int32_t GetWidth() const noexcept { return width_; }
  int32_t GetHeight() const noexcept { return height_; }
  int32_t GetSize() const noexcept { return width_ * height_; }
  Buffer& GetData() noexcept { return buffer_; }

  float GetDepth(int32_t x, int32_t y);
  void SetDepth(int32_t x, int32_t y, float depth);

 private:
  Buffer buffer_;
  int32_t width_{};
  int32_t height_{};
};

struct ConstantBufferData {
  mat4 matWorld;
  mat4 matView;
  mat4 matProjection;
  const std::unordered_map<std::string, std::any>* properties;
};

class ConstantBuffer {
 public:
  ConstantBuffer();
  ConstantBuffer(ConstantBufferData data);
  ~ConstantBuffer() = default;

  void SetData(ConstantBufferData data) noexcept;
  void SetWorldMatrix(mat4 matWorld) noexcept;
  void SetViewMatrix(mat4 matView) noexcept;
  void SetProjectionMatrix(mat4 matProjection) noexcept;
  void SetProperties(
      const std::unordered_map<std::string, std::any>* properties);

  mat4 GetWorldMatrix() const noexcept;
  mat4 GetViewMatrix() const noexcept;
  mat4 GetProjectionMatrix() const noexcept;
  const std::unordered_map<std::string, std::any>* GetProperties()
      const noexcept;

 private:
  const ConstantBufferData* Get() const noexcept;
  ConstantBufferData* Get() noexcept;

  Buffer buffer_;
};
}  // namespace softy

#endif  // RENDER_BUFFER_H_