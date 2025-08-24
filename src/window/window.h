#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "event/event_channel.h"
#include "render/buffer.h"

namespace softy {
struct WindowDescriptor {
  std::string name;
  int32_t width;
  int32_t height;
};

class Window {
 public:
  Window();
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  bool Create(WindowDescriptor& descriptor, EventChannel& eventChannel,
              Buffer& colorBuffer);
  bool Update();
  void Present();
  void SetTitle(std::string_view title);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace softy

#endif  // WINDOW_WINDOW_H_