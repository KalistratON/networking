#ifndef ABSTRACTSOCKETIMPL_HPP
#define ABSTRACTSOCKETIMPL_HPP

#include "NetworkingTypes.hpp"

namespace networking {
namespace internal {

class AbstractSocketImpl {

public:
    AbstractSocketImpl (SocketType theSocket);
    SocketType GetSocket () const;

public:
    SocketType mySocket;
};

}
}

#endif