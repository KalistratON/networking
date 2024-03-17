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
#include <sys/epoll.h>
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
                SocketType aSocket = aSET[i].fd;
                if (aSocket == myMasterSocket->GetSocket()) {
                    const auto& aClientSocket = myMasterSocket->Accept();
                    myClientSockets.emplace (aClientSocket);
                    aSET[++aCurrentIndex].fd = aClientSocket->GetSocket();
                    aSET[0].events = POLLIN;
                    
                    {
                        myTcpConnectionEvent = TcpConnectionEvent (aClientSocket);
                        myTcpConnectionEvent();
                    }

                } else {
                    
                    {
                        myTcpMsgEvent = TcpMsgEvent (*myClientSockets.find (std::make_shared<TcpSocket>(aSocket)));
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

    aMasterEvent.data.fd = myMasterSocket->GetSocket();
    aMasterEvent.events = EPOLLIN;
    epoll_ctl (EPOLL, EPOLL_CTL_ADD,  myMasterSocket->GetSocket(), &aMasterEvent);


    std::cout << "Socket : " << myMasterSocket->GetSocket() << std::endl;

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
            SocketType aSocket = events[i].data.fd;
            if (aSocket == myMasterSocket->GetSocket()) {
                const auto& aClientSocket = myMasterSocket->Accept();
                std::cout << N << " : socket : " << aClientSocket->GetSocket() << std::endl;
                epoll_event aClientEvent;
                aClientEvent.data.fd = aClientSocket->GetSocket();
                aClientEvent.events = EPOLLIN;

                myClientSockets.emplace (aClientSocket);
                
                if (epoll_ctl (EPOLL, EPOLL_CTL_ADD,  aClientSocket->GetSocket(), &aClientEvent) == -1) {
                    perror("epoll_ctl: listen_sock");
                    exit(EXIT_FAILURE);
                }

                {
                    myTcpConnectionEvent = TcpConnectionEvent (aClientSocket);
                    myTcpConnectionEvent();
                    
                    std::cout << "Event connection was emit" << std::endl;
                }

            } else {

                // processing of income messages;
                {
                    char aData [100];
                    int aReadReturned = myClientSockets.at(i)->Read (aData, 100);
                    if (aReadReturned > 0) {
                        
                        std::cout << "Message | " << aData << std::endl;

                        myTcpMsgEvent = TcpMsgEvent (myClientSockets[i]);
                        myTcpMsgEvent();

                        std::cout << "Event msg was emit" << std::endl;

                    } else if (aReadReturned == 0 && errno != EAGAIN) {
                    
                        std::cout << "Close" << std::endl;

                        myTcpMsgEvent = TcpMsgEvent (myClientSockets[i]);
                        myTcpMsgEvent();

                        std::cout << "Event msg was emit" << std::endl;
                        const auto& aClientSocket = *myClientSockets.find (std::make_shared<TcpSocket>(aSocket))
                        epoll_ctl (EPOLL, EPOLL_CTL_DEL,  aClientSocket->GetSocket(), NULL);
                        myClientSockets.erase (std::make_shared<TccpSocket>(aSocket));
                    }
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

void TcpServer::TcpServer::Close()
{
    myImpl->Close();
}