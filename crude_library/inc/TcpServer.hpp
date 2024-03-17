#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "TcpSocket.hpp"

#include "../../event_system/inc/AbstractEventHandler.hpp"

#include <cstdint>
#include <memory>

namespace networking {

namespace internal {
    class TcpServerImpl;
}

class TcpServer {
public:
    _NETWORKING_EXPORT TcpServer (const char* theReciverAddress = "127.0.0.1", 
                                  std::uint16_t thePort = 1);

    _NETWORKING_EXPORT bool Listen();
    _NETWORKING_EXPORT void Close();

    _NETWORKING_EXPORT void 
    RegisterConnectionHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler);
    _NETWORKING_EXPORT void 
    UnregisterConnectionHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler);

    _NETWORKING_EXPORT void 
    RegisterMsgHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler);
    _NETWORKING_EXPORT void 
    UnregisterMsgHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler);
    
private:
/*emit events*/

private:
    std::shared_ptr<internal::TcpServerImpl> myImpl;
};
}

#endif