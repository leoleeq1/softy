#ifndef EVENT_EVENT_CHANNEL_H_
#define EVENT_EVENT_CHANNEL_H_

#include <any>
#include <functional>
#include <unordered_map>

#include "event/event_handler.h"
#include "type/type_id.h"

namespace softy {
class EventChannel {
 public:
  EventChannel() = default;
  ~EventChannel() = default;

  template <typename TEvent>
  void Send(const TEvent& event);

  template <typename TEvent>
  void Subscribe(EventHandler handler);

  template <typename TEvent>
  void Unsubscribe(EventHandler handler);

 private:
  std::unordered_map<uint32_t, std::vector<EventHandler>> subscribers_;
};

template <typename TEvent>
inline void EventChannel::Send(const TEvent& event) {
  constexpr uint32_t id = type_id<TEvent>::value;
  if (subscribers_.contains(id)) {
    for (const auto& callback : subscribers_[id]) {
      callback(std::any{event});
    }
  }
}

template <typename TEvent>
inline void EventChannel::Subscribe(EventHandler handler) {
  constexpr uint32_t id = type_id<TEvent>::value;
  if (!subscribers_.contains(id)) {
    subscribers_[id] = {};
  }
  subscribers_[id].push_back(handler);
}

template <typename TEvent>
inline void EventChannel::Unsubscribe(EventHandler handler) {
  constexpr uint32_t id = type_id<TEvent>::value;
  if (subscribers_.contains(id)) {
    subscribers_[id].erase(
        std::remove(subscribers_[id].begin(), subscribers_[id].end(), handler),
        subscribers_[id].end());
  }
}
}  // namespace softy

#endif  // EVENT_EVENT_CHANNEL_H_