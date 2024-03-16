#include "../inc/AbstractEvent.hpp"

using namespace networking::event_system;

AbstractEvent::AbstractEvent() : myHandlers() 
{   
};

void AbstractEvent::operator()() const
{
    for(const auto& aHandler : myHandlers) {
        aHandler->call (this);
    }
}

void AbstractEvent::operator += (const Handler theHandler)
{
    myHandlers.emplace (theHandler);
}

void AbstractEvent::operator -= (const Handler theHandler)
{
    myHandlers.erase (theHandler);
}