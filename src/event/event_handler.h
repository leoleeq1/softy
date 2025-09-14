#ifndef EVENT_EVENT_HANDLER_H_
#define EVENT_EVENT_HANDLER_H_

#include <any>
#include <cstdint>
#include <functional>

namespace softy {
class EventHandler {
 public:
  using CallbackType = std::function<void(std::any)>;

  EventHandler(CallbackType callback);

  void operator()(std::any value) const { callback_(value); }

  bool operator==(const EventHandler& rhs) const { return id_ == rhs.id_; }

 private:
  CallbackType callback_;
  uint32_t id_;
};
}  // namespace softy

#endif  // EVENT_EVENT_HANDLER_H_