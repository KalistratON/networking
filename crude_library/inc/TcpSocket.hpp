#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include "AbstractSocket.hpp"

#include <memory>

struct sockaddr_in;

namespace networking {

namespace internal {
    class TcpSocketImpl;
    class TcpServerImpl;
}

class TcpSocket : AbstractSocket {
public:
    _NETWORKING_EXPORT TcpSocket (SocketType theSocket = -1);

    _NETWORKING_EXPORT bool Bind (const char* theReciverAddress = "127.0.0.1", 
                                  std::uint16_t thePort = 1);

    _NETWORKING_EXPORT bool Listen();
    _NETWORKING_EXPORT bool Connect (const sockaddr_in& theServerInfo);

    _NETWORKING_EXPORT std::shared_ptr <TcpSocket> Accept();
    _NETWORKING_EXPORT std::shared_ptr <TcpSocket> Accept (sockaddr_in& theSenderInfo);

    _NETWORKING_EXPORT bool Read  (const TcpSocket& theSocket, 
                                   char* theData, std::uint64_t theMaxDataSize);
    _NETWORKING_EXPORT bool Write (const TcpSocket& theSocket, 
                                   const char* theData, std::uint64_t theDataSize);
    _NETWORKING_EXPORT void Close();


    friend internal::TcpServerImpl;

private:
    std::shared_ptr <internal::TcpSocketImpl> myImpl;
};
}

#endif