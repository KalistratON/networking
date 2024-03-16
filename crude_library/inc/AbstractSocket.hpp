#ifndef ABSTRACTSOCKET_HPP
#define ABSTRACTSOCKET_HPP

#include "NetworkingTypes.hpp"

#include <memory>

namespace networking {

namespace internal {
class AbstractSocketImpl;
}

class AbstractSocket {

using Impl    = internal::AbstractSocketImpl;
using ImplPtr = std::shared_ptr<internal::AbstractSocketImpl>;

public:
    _NETWORKING_EXPORT AbstractSocket (const ImplPtr& theImpl);
    _NETWORKING_EXPORT SocketType GetSocket () const;

    _NETWORKING_EXPORT operator bool() const;
    _NETWORKING_EXPORT bool operator == (const AbstractSocket& theCmp) const;
    _NETWORKING_EXPORT bool operator != (const AbstractSocket& theCmp) const;

protected:
    ImplPtr myImpl;
};

}

#endif