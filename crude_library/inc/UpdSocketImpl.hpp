#ifndef UPDSOCKETIMPL_HPP
#define UPDSOCKETIMPL_HPP

#include "AbstractSocketImpl.hpp"

#include <cstdint>

struct sockaddr_in;

namespace networking {
namespace internal {

class UpdSocketImpl : public AbstractSocketImpl {
public:
    UpdSocketImpl(SocketType theSocket);
    virtual bool Bind (const char* theReciverAddress, std::uint16_t thePort);
    
    bool ReadDatagram (char* theData, std::uint64_t theMaxDataSize);
    bool ReadDatagram (char* theData, std::uint64_t theMaxDataSize, sockaddr_in& theSenderInfo);
    bool WriteDatagram (const char* theData, std::uint64_t theDataSize, 
                        const char* theReciverAddress, std::uint16_t thePort);
};

}
}

#endif