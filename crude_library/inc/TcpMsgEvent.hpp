#ifndef TCPMSGEVENT_HPP
#define TCPMSGEVENT_HPP

#include "../../event_system/inc/AbstractEvent.hpp"
#include "../inc/TcpSocket.hpp"

#include <memory>

namespace networking {

class TcpMsgEvent : public event_system::AbstractEvent {

public:
    TcpMsgEvent();
    TcpMsgEvent (std::shared_ptr<TcpSocket> theTcpSocket);
    const std::shared_ptr<TcpSocket>& get() const;

private:
    std::shared_ptr<TcpSocket> myTcpSocket;
};

}

#endif