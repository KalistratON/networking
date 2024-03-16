#ifndef ABSTRACTEVENT_HPP
#define ABSTRACTEVENT_HPP

#include "../inc/AbstractEventHandler.hpp"
#include "../inc/EventMacro.hpp"

#include <memory>
#include <unordered_set>

namespace networking {
namespace event_system {

using Handler = std::shared_ptr <AbstractEventHandler>;

class AbstractEvent
{
struct EventHandlerHash {
    size_t operator() (Handler theHandler) const
    {
        return reinterpret_cast<size_t> (theHandler.get());
    }
};

struct EventHandlerEqual {
    bool operator() (const Handler theHandler1, const Handler theHandler2) const
    {
        return theHandler1 == theHandler2;
    }
};

public:
    _EVENT_EXPORT AbstractEvent();
    _EVENT_EXPORT void operator()() const;
    _EVENT_EXPORT void operator += (const Handler theHandler);
    _EVENT_EXPORT void operator -= (const Handler theHandler);

private:
    std::unordered_set <Handler, EventHandlerHash, EventHandlerEqual> myHandlers;
};

}
}

#endif