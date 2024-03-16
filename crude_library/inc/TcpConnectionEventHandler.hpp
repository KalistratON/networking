#ifndef TCPCONNECTIONEVENTHANDLER_HPP
#define TCPCONNECTIONEVENTHANDLER_HPP

#include "../../event_system/inc/AbstractEvent.hpp"
#include "../../event_system/inc/AbstractEventHandler.hpp"
#include "../inc/TcpSocket.hpp"

#include <memory>

namespace networking {

class TcpConnectionEventHandler : public event_system::AbstractEventHandler {

public:
    TcpConnectionEventHandler();
    virtual void call (const event_system::AbstractEvent* theEvent) const override;

private:
    std::shared_ptr<TcpSocket> myTcpSocket;
};

}

#endif