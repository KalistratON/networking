#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "TcpSocket.hpp"

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

    _NETWORKING_EXPORT const TcpSocket& NextPendingConnection();

private:
    std::shared_ptr<internal::TcpServerImpl> myImpl;
};
}

#endif