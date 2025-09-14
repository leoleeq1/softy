#ifndef EVENT_WINDOW_EVENT_H_
#define EVENT_WINDOW_EVENT_H_

#include "input/keycode.h"

namespace softy {
struct WindowCreatedEvent {};
struct WindowKeyDownEvent {
  KeyCode key;
};
struct WindowKeyUpEvent {
  KeyCode key;
};
}  // namespace softy

#endif  // EVENT_WINDOW_EVENT_H_