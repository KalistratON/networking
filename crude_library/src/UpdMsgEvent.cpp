#include "../inc/UpdMsgEvent.hpp"

using namespace networking;
using namespace event_system;

UpdMsgEvent::UpdMsgEvent() {};

UpdMsgEvent::UpdMsgEvent (std::shared_ptr<UpdSocket> theUpdSocket) :
    myUpdSocket (theUpdSocket)
{}

const std::shared_ptr<UpdSocket>& UpdMsgEvent::get() const
{
    return myUpdSocket;
}