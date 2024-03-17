#ifndef TCPSERVERIMPL_HPP
#define TCPSERVERIMPL_HPP

#include "TcpSocket.hpp"
#include "TcpConnectionEvent.hpp"
#include "TcpMsgEvent.hpp"

#include <cstdint>
#include <unordered_set>

namespace networking {
namespace internal {
    
class TcpServerImpl {

    struct SocketHash {
        size_t operator () (const std::shared_ptr<TcpSocket>& theSocket) {
            return theSocket->GetSocket();
        }
    };

    struct SocketEqual {
        bool operator () (const std::shared_ptr<TcpSocket>& theS1, const std::shared_ptr<TcpSocket>& theS2) {
            return theS1->GetSocket() == theS2->GetSocket();
        }
    };

    using TcpSocketUSetType = std::unordered_set<std::shared_ptr<TcpSocket>, SocketHash, SocketEqual>;

public:
    TcpServerImpl (const char* theReciverAddress, std::uint16_t thePort);

    bool Listen();
    void Close();

private:
    void MultiplexingProcessing();

private:
    TcpSocketUSetType myClientSockets;
    std::shared_ptr<TcpSocket> myMasterSocket;

/*events section*/
public:
    static TcpConnectionEvent myTcpConnectionEvent;
    static TcpMsgEvent myTcpMsgEvent;
};

}
}

#endif