#include "input/input_manager.h"

#include <any>

#include "event/event_channel.h"
#include "event/event_handler.h"
#include "event/window_event.h"
#include "input/keycode.h"
#include "input_manager.h"

namespace softy {
InputManager::InputManager(EventChannel& channel)
    : channel_{channel},
      onKeyDownEvent_{[this](std::any evt) {
        WindowKeyDownEvent e = std::any_cast<WindowKeyDownEvent>(evt);
        keyStates[static_cast<int>(e.key)] = true;
      }},
      onKeyUpEvent_{[this](std::any evt) {
        WindowKeyUpEvent e = std::any_cast<WindowKeyUpEvent>(evt);
        keyStates[static_cast<int>(e.key)] = false;
      }} {
  channel_.Subscribe<WindowKeyDownEvent>(onKeyDownEvent_);
  channel_.Subscribe<WindowKeyUpEvent>(onKeyUpEvent_);
}

InputManager::~InputManager() {
  channel_.Unsubscribe<WindowKeyDownEvent>(onKeyDownEvent_);
  channel_.Unsubscribe<WindowKeyUpEvent>(onKeyUpEvent_);
}

bool InputManager::GetKey(KeyCode key) const noexcept {
  return keyStates[static_cast<int>(key)];
}
}  // namespace softy