#include "../inc/UpdMsgEvent.hpp"
#include "../inc/UpdMsgEventHandler.hpp"

#include <memory>

using namespace networking;
using namespace event_system;

UpdMsgEventHandler::UpdMsgEventHandler() {};

void UpdMsgEventHandler::call (const AbstractEvent* /*theEvent*/) const
{};