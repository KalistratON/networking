#ifndef TCPSERVERIMPL_HPP
#define TCPSERVERIMPL_HPP

#include "TcpSocket.hpp"

#include <cstdint>
#include <vector>

namespace networking {
namespace internal {
    
class TcpServerImpl {
    using TcpSocketVecType = std::vector<TcpSocket>;
public:
    TcpServerImpl (const char* theReciverAddress, std::uint16_t thePort);

    bool Listen();
    void Close();
    const TcpSocket& NextPendingConnection();

private:
    void MultiplexingProcessing();

private:
    TcpSocketVecType myClientSockets;
    TcpSocket myMasterSocket;
};

}
}

#endif