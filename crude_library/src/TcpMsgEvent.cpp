#include "../inc/TcpMsgEvent.hpp"

using namespace networking;
using namespace event_system;

TcpMsgEvent::TcpMsgEvent() {};

TcpMsgEvent::TcpMsgEvent (std::shared_ptr<TcpSocket> theTcpSocket) :
    myTcpSocket (theTcpSocket)
{}

const std::shared_ptr<TcpSocket>& TcpMsgEvent::get() const
{
    return myTcpSocket;
}