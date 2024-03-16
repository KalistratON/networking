#ifndef TCPCONNECTIONEVENT_HPP
#define TCPCONNECTIONEVENT_HPP

#include "../../event_system/inc/AbstractEvent.hpp"
#include "../../event_system/inc/AbstractEventHandler.hpp"
#include "../inc/TcpSocket.hpp"

#include <memory>

namespace networking {

class TcpConnectionEvent : public event_system::AbstractEvent {

public:

    TcpConnectionEvent ();
    TcpConnectionEvent (std::shared_ptr<TcpSocket> theTcpSocket);
    const std::shared_ptr<TcpSocket>& get() const;

private:
    std::shared_ptr<TcpSocket> myTcpSocket;
};

}

#endif