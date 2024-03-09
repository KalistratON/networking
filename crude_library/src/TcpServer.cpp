#include "../inc/TcpServer.hpp"
#include "../inc/TcpServerImpl.hpp"
#include "../inc/TcpSocketImpl.hpp"

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

using namespace networking;

void internal::TcpServerImpl::MultiplexingProcessing()
{
#ifdef _WIN32 // poll
    const std::uint16_t aMaxConnectedClients = 2048;
    pollfd aSET [aMaxConnectedClients];

    size_t aCurrentIndex = 0;
    aSET[0].fd = myMasterSocket.myImpl->mySocket;
    aSET[0].events = POLLIN;

    WSAPoll (aSET, aMaxConnectedClients, -1);
    for (size_t i = 0; i < aCurrentIndex; ++i) {
        if (aSET[i].revents & POLLIN) {
            if (!i) {
                const auto& aClientSocket = myMasterSocket.Accept();
                myClientSockets.push_back (*aClientSocket);
                aSET[++aCurrentIndex].fd = aClientSocket->myImpl->mySocket;
                aSET[0].events = POLLIN;

                // new connection signal emitting
            } else {

            }
        }
    }
#endif
}

internal::TcpServerImpl::TcpServerImpl (const char* theReciverAddress, std::uint16_t thePort)
{
    myMasterSocket.Bind (theReciverAddress, thePort);
}

void internal::TcpServerImpl::Close()
{
    for (auto& aSocket : myClientSockets) {
        aSocket.Close();
    }
    myClientSockets.clear();
    myMasterSocket.Close();
}

bool internal::TcpServerImpl::Listen()
{
    MultiplexingProcessing();
    return true;
}

const TcpSocket& internal::TcpServerImpl::NextPendingConnection()
{
    return myClientSockets.back();
}

TcpServer::TcpServer::TcpServer(const char* theReciverAddress, std::uint16_t thePort) :
    myImpl(std::make_shared<internal::TcpServerImpl>(theReciverAddress, thePort))
{}

bool TcpServer::TcpServer::Listen()
{
   return myImpl->Listen();
}

const TcpSocket& TcpServer::TcpServer::NextPendingConnection()
{
    return myImpl->NextPendingConnection();
}

void TcpServer::TcpServer::Close()
{
    myImpl->Close();
}