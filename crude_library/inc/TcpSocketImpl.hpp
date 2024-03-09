#ifndef TCPSOCKETIMPL_HPP
#define TCPSOCKETIMPL_HPP

#include "AbstractSocketImpl.hpp"

#include <cstdint>

struct sockaddr_in;

namespace networking {
namespace internal {

class TcpServerImpl;

class TcpSocketImpl : public AbstractSocketImpl {
public:
    TcpSocketImpl (SocketType theSocket);
    bool Bind (const char* theReciverAddress, std::uint16_t thePort);

    bool Listen();
    bool Connect (const sockaddr_in& theServerInfo);

    SocketType Accept ();
    SocketType Accept (sockaddr_in& theServerInfo);
    
    bool Read  (const std::shared_ptr<TcpSocketImpl>& theSocket, char* theData, 
                std::uint64_t theMaxDataSize);
    bool Write (const std::shared_ptr<TcpSocketImpl>& theSocket, const char* theData, 
                std::uint64_t theDataSize);
    void Close();


    friend TcpServerImpl;
};

}
}

#endif