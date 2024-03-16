#ifndef UPDSOCKET_HPP
#define UPDSOCKET_HPP

#include "AbstractSocket.hpp"

#include <memory>

// used both in Windows and Linux
struct sockaddr_in;

namespace networking {

namespace internal {
    class UpdSocketImpl;
}

class UpdSocket : public AbstractSocket {

public:
    _NETWORKING_EXPORT UpdSocket (SocketType theSocket = -1);

    _NETWORKING_EXPORT bool Bind (const char* theReciverAddress, std::uint16_t thePort);
    
    _NETWORKING_EXPORT bool ReadDatagram (char* theData, std::uint64_t theMaxDataSize);
    _NETWORKING_EXPORT bool ReadDatagram (char* theData, std::uint64_t theMaxDataSize, 
                                          sockaddr_in& theSenderInfo);
    _NETWORKING_EXPORT bool WriteDatagram (const char* theData, std::uint64_t theDataSize,
                                           const char* theReciverAddress = "127.0.0.1", 
                                           std::uint16_t thePort = 1);
};
}

#endif