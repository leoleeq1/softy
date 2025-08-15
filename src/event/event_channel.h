#ifndef EVENT_EVENT_CHANNEL_H_
#define EVENT_EVENT_CHANNEL_H_

#include <functional>
#include <unordered_map>

#include "type/type_id.h"

namespace softy {
class EventChannel {
 public:
  EventChannel() = default;
  ~EventChannel() = default;

  template <typename TEvent>
  void Send(const TEvent& event);

 private:
  std::unordered_map<uint32_t, std::function<void(const void*)>> subscribers_;
};

template <typename TEvent>
inline void EventChannel::Send(const TEvent& event) {
  constexpr uint32_t id = type_id<TEvent>::value;
  if (subscribers_.contains(id)) {
    subscribers_.at(id)(static_cast<const void*>(&event));
  }
}
}  // namespace softy

#endif  // EVENT_EVENT_CHANNEL_H_