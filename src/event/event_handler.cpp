#include "event/event_handler.h"

#include <any>
#include <cstdint>
#include <functional>

namespace softy {
static uint32_t idGenerator{};

softy::EventHandler::EventHandler(CallbackType callback)
    : callback_{callback}, id_{idGenerator++} {}
}  // namespace softy