#ifndef TCPSERVERIMPL_HPP
#define TCPSERVERIMPL_HPP

#include "TcpSocket.hpp"
#include "TcpConnectionEvent.hpp"
#include "TcpMsgEvent.hpp"

#include <cstdint>
#include <vector>

namespace networking {
namespace internal {
    
class TcpServerImpl {
    using TcpSocketVecType = std::vector<std::shared_ptr<TcpSocket>>;

public:
    TcpServerImpl (const char* theReciverAddress, std::uint16_t thePort);

    bool Listen();
    void Close();
    const std::shared_ptr<TcpSocket>& NextPendingConnection();

private:
    void MultiplexingProcessing();

private:
    TcpSocketVecType myClientSockets;
    std::shared_ptr<TcpSocket> myMasterSocket;

/*events section*/
public:
    static TcpConnectionEvent myTcpConnectionEvent;
    static TcpMsgEvent myTcpMsgEvent;
};

}
}

#endif