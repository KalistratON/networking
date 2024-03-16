#include "../inc/TcpConnectionEvent.hpp"

using namespace networking;
using namespace event_system;

TcpConnectionEvent::TcpConnectionEvent () {};

TcpConnectionEvent::TcpConnectionEvent (std::shared_ptr<TcpSocket> theTcpSocket) :
    myTcpSocket (theTcpSocket)
{}

const std::shared_ptr<TcpSocket>& TcpConnectionEvent::get() const
{
    return myTcpSocket;
}