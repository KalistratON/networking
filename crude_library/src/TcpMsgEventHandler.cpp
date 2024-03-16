#include "../inc/TcpMsgEvent.hpp"
#include "../inc/TcpMsgEventHandler.hpp"

#include <memory>

using namespace networking;
using namespace event_system;

TcpMsgEventHandler::TcpMsgEventHandler() {};

void TcpMsgEventHandler::call (const AbstractEvent* /*theEvent*/) const
{};