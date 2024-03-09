#include "../inc/TcpSocket.hpp"
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
                  << WSAGetLastError();
#endif
}

internal::TcpSocketImpl::TcpSocketImpl (SocketType theSocket) :
    AbstractSocketImpl(theSocket)
{};

bool internal::TcpSocketImpl::Bind (const char* theReciverAddress, std::uint16_t thePort)
{
#ifdef _WIN32
    WSADATA wsData;
    int anError = WSAStartup (MAKEWORD (2, 2), &wsData);
    if (anError != 0) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
#endif
    
    mySocket = socket (AF_INET, SOCK_STREAM, 0);
    if (mySocket == INVALID_SOCKET) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }

    sockaddr_in aSocketInfo;
    {
        in_addr ip_to_num;
        if (inet_pton (AF_INET, theReciverAddress, &ip_to_num) == -1) {
            ErrorProcessing (__LINE__, __FILE__);
        }

        aSocketInfo.sin_family = AF_INET;
        aSocketInfo.sin_addr = ip_to_num;
        aSocketInfo.sin_port = htons (thePort);
    }
    int aBindReturn = bind (mySocket, reinterpret_cast <const sockaddr*> (&aSocketInfo), 
                             sizeof (aSocketInfo));
    if (aBindReturn <= 0) {
        ErrorProcessing (__LINE__, __FILE__);
    }
    return aBindReturn;
}

bool internal::TcpSocketImpl::Listen ()
{
    if (listen (mySocket, SOMAXCONN) == SOCKET_ERROR) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
    return true;
}

SocketType internal::TcpSocketImpl::Accept()
{
    const auto& aClientSocket = accept (mySocket, nullptr, nullptr);
    return aClientSocket;
}

SocketType internal::TcpSocketImpl::Accept (sockaddr_in& theServerInfo)
{
    int aSenderInfoSize = sizeof (theServerInfo);
    const auto& aClientSocket = accept (mySocket, (sockaddr*) &theServerInfo, &aSenderInfoSize); // make socket here
    return aClientSocket;
}

bool internal::TcpSocketImpl::Connect (const sockaddr_in& theServerInfo)
{
    return connect (mySocket, (sockaddr*) &theServerInfo, sizeof (theServerInfo));
}

void internal::TcpSocketImpl::Close()
{
    shutdown (mySocket, SD_BOTH);
    closesocket (mySocket);
}

bool internal::TcpSocketImpl::Read (const std::shared_ptr<TcpSocketImpl>& theSocket, 
                                    char* theData, std::uint64_t theMaxDataSize)
{
    int aRecvReturn = recv (theSocket->mySocket, theData, static_cast <int> (theMaxDataSize), 0);
    if (aRecvReturn == SOCKET_ERROR) {
        ErrorProcessing(__LINE__, __FILE__);
#ifdef _WIN32
        if (aRecvReturn == WSAENETDOWN
            || aRecvReturn == WSAENOTCONN
            || aRecvReturn == WSAENOTSOCK
            || aRecvReturn == WSAETIMEDOUT) {
            Close();
        }
        return false;
#endif
    }
    return true;
}

bool internal::TcpSocketImpl::Write (const std::shared_ptr<TcpSocketImpl>& theSocket, 
                                     const char* theData, std::uint64_t theDataSize)
{
    return send (theSocket->mySocket, theData, static_cast<int> (theDataSize), 0);
}

TcpSocket::TcpSocket (SocketType theSocket) :
    AbstractSocket (theSocket),
    myImpl(std::make_shared<internal::TcpSocketImpl> (theSocket))
{};

bool TcpSocket::Bind (const char* theReciverAddress, std::uint16_t thePort)
{
    return myImpl->Bind (theReciverAddress, thePort);
}

bool TcpSocket::Listen()
{
    return myImpl->Listen();
}

std::shared_ptr<TcpSocket> TcpSocket::Accept()
{
    const auto& aClientSocket = myImpl->Accept();
    return std::make_shared<TcpSocket> (aClientSocket);
}

std::shared_ptr<TcpSocket> TcpSocket::Accept (sockaddr_in& theSenderInfo)
{
    const auto& aClientSocket = myImpl->Accept (theSenderInfo);
    return std::make_shared<TcpSocket> (aClientSocket);
}

bool TcpSocket::Connect (const sockaddr_in& theServerInfo)
{
    return myImpl->Connect (theServerInfo);
}

bool TcpSocket::Read (const TcpSocket& theSocket, char* theData, std::uint64_t theMaxDataSize)
{
    return myImpl->Read (theSocket.myImpl, theData, theMaxDataSize);
}

bool TcpSocket::Write (const TcpSocket& theSocket, const char* theData, std::uint64_t theDataSize)
{
    return myImpl->Write (theSocket.myImpl, theData, theDataSize);
}

void TcpSocket::Close()
{
    myImpl->Close();
}

