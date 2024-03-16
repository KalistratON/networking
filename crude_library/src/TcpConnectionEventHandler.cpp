#include "../inc/TcpConnectionEvent.hpp"
#include "../inc/TcpConnectionEventHandler.hpp"

#include <memory>

using namespace networking;
using namespace event_system;

TcpConnectionEventHandler::TcpConnectionEventHandler() {};

void TcpConnectionEventHandler::call (const AbstractEvent* /*theEvent*/) const 
{};