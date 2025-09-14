#ifndef INPUT_INPUT_MANAGER_H_
#define INPUT_INPUT_MANAGER_H_

#include <array>

#include "event/event_channel.h"
#include "event/event_handler.h"
#include "event/window_event.h"
#include "input/keycode.h"

namespace softy {
class InputManager {
 public:
  InputManager(EventChannel& channel);
  ~InputManager();

  bool GetKey(KeyCode key) const noexcept;

 private:
  EventChannel& channel_;
  std::array<bool, 256> keyStates{};
  EventHandler onKeyDownEvent_;
  EventHandler onKeyUpEvent_;
};
}  // namespace softy

#endif  // INPUT_INPUT_MANAGER_H_