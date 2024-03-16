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

#include <iostream>

using namespace networking;

static void ErrorProcessing (int theLine, const char* theFile)
{
#ifdef _WIN32
        std::cerr << "Error! | " 
                  << "LINE : " << theLine << " : FILE : "  << theFile 
                  << "Error Code : " << WSAGetLastError();
#endif
}

TcpConnectionEvent internal::TcpServerImpl::myTcpConnectionEvent = TcpConnectionEvent();
TcpMsgEvent internal::TcpServerImpl::myTcpMsgEvent = TcpMsgEvent();

void internal::TcpServerImpl::MultiplexingProcessing()
{
#ifdef _WIN32 // poll
    const std::uint16_t aMaxConnectedClients = 2048;
    pollfd aSET [aMaxConnectedClients];
    FD_ZERO (&aSET); 

    size_t aCurrentIndex = 1;
    aSET[0].fd = myMasterSocket->GetSocket();
    aSET[0].events = POLLIN;

    while (true) {
        int error = WSAPoll (aSET, 1, -1);
        if (error == SOCKET_ERROR) {
            ErrorProcessing (__LINE__, __FILE__);
        }
        for (size_t i = 0; i < aCurrentIndex; ++i) {
            if (aSET[i].revents & POLLIN) {
                if (!i) {
                    const auto& aClientSocket = myMasterSocket->Accept();
                    myClientSockets.push_back (aClientSocket);
                    aSET[++aCurrentIndex].fd = aClientSocket->GetSocket();
                    aSET[0].events = POLLIN;
                    
                    {
                        myTcpConnectionEvent = TcpConnectionEvent (aClientSocket);
                        myTcpConnectionEvent();
                    }

                } else {

                    
                    {
                        myTcpMsgEvent = TcpMsgEvent (myClientSockets[i]);
                        myTcpMsgEvent();
                    }
                }
            }
        }
    }
#elif __linux__
    const std::uint16_t aMaxConnectedClients = 2048;
    int EPOLL = epoll_create1(0);
    epoll_event aMasterEvent;

    aMasterEvent.data.fd = myMasterSocket.myImpl->mySocket;
    aMasterEvent.events = EPOLLIN;
    epoll_ctl (EPOLL, EPOLL_CTL_ADD,  myMasterSocket.myImpl->mySocket, &aMasterEvent);


    std::cout << "Socket : " << myMasterSocket.myImpl->mySocket << std::endl;

    while (true) {
        epoll_event events [aMaxConnectedClients];
        int N = epoll_wait (EPOLL, events, aMaxConnectedClients, -1);
        // std::cout << "After wait" << std::endl;
        if (N == -1) {
            std::cout << "Error is occured" << std::endl;
        } else if (N == 0) {
            std::cout << "Error = 0 is occured" << std::endl;
        }
        
        for (size_t i = 0; i < N; ++i) {
            if (events[i].data.fd == myMasterSocket.myImpl->mySocket) {
                const auto& aClientSocket = myMasterSocket.Accept();
                std::cout << N << " : socket : " << aClientSocket->myImpl->mySocket << std::endl;
                epoll_event aClientEvent;
                aClientEvent.data.fd = aClientSocket->myImpl->mySocket;
                aClientEvent.events = EPOLLIN;
                epoll_ctl (EPOLL, EPOLL_CTL_ADD,  aClientSocket->myImpl->mySocket, &aClientEvent);

                {
                    myTcpConnectionEvent = TcpConnectionEvent (aClientSocket);
                    myTcpConnectionEvent();
                }

            } else {
                // processing of income messages;
                {
                    myTcpMsgEvent = TcpMsgEvent (myClientSockets[i]);
                    myTcpMsgEvent();
                }
            }
        }
    }
    
#endif
}

void TcpServer::RegisterMsgHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler)
{
    myImpl->myTcpMsgEvent += theHandler;
}

void TcpServer::UnregisterMsgHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler)
{
    myImpl->myTcpMsgEvent -= theHandler;
}

void TcpServer::RegisterConnectionHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler)
{
    myImpl->myTcpConnectionEvent += theHandler;
}

void TcpServer::UnregisterConnectionHandler (const std::shared_ptr <event_system::AbstractEventHandler>& theHandler)
{
    myImpl->myTcpConnectionEvent -= theHandler;
}

internal::TcpServerImpl::TcpServerImpl (const char* theReciverAddress, std::uint16_t thePort) :
    myMasterSocket (std::make_shared<TcpSocket>())
{
    myMasterSocket->Bind (theReciverAddress, thePort);
}

void internal::TcpServerImpl::Close()
{
    for (auto& aSocket : myClientSockets) {
        aSocket->Close();
    }
    myClientSockets.clear();
    myMasterSocket->Close();
}

bool internal::TcpServerImpl::Listen()
{
    myMasterSocket->Listen();
    MultiplexingProcessing();
    return true;
}

const std::shared_ptr<TcpSocket>& internal::TcpServerImpl::NextPendingConnection()
{
    return myClientSockets.back();
}

TcpServer::TcpServer::TcpServer(const char* theReciverAddress, std::uint16_t thePort) :
    myImpl (std::make_shared<internal::TcpServerImpl> (theReciverAddress, thePort))
{}

/*
* Provide TCP server which emit signals when new connection was recived.
* Standart workflow with it : 
    1. catch signals of new connection OR new message
    2. proccesing them by TcpSocket interface

    all sockets stores in TcpServer class. Users only need to process msg and get answer of it or Set processing function
*/

bool TcpServer::TcpServer::Listen()
{
   return myImpl->Listen();
}

const std::shared_ptr<TcpSocket>& TcpServer::TcpServer::NextPendingConnection()
{
    return myImpl->NextPendingConnection();
}

void TcpServer::TcpServer::Close()
{
    myImpl->Close();
}