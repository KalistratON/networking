#ifndef ABSTRACTEVENTHANDLER_HPP
#define ABSTRACTEVENTHANDLER_HPP

#include "../inc/EventMacro.hpp"

#include <memory>

namespace networking {
namespace event_system {

class AbstractEvent;

class AbstractEventHandler
{
public:
    _EVENT_EXPORT virtual void call (const AbstractEvent* theEvent) const = 0;
};

}
}

#endif