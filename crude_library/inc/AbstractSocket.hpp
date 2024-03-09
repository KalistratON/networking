#ifndef ABSTRACTSOCKET_HPP
#define ABSTRACTSOCKET_HPP

#include "NetworkingTypes.hpp"

#include <memory>

namespace networking {

namespace internal {
class AbstractSocketImpl;
}

class AbstractSocket {
public:
    _NETWORKING_EXPORT AbstractSocket (SocketType theSocket);

private:
    std::shared_ptr <internal::AbstractSocketImpl> myImpl;
};

}

#endif