#include "../inc/TcpSocket.hpp"
#include "../inc/TcpSocketImpl.hpp"
#include "../inc/Helper.hpp"

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include <iostream>

using namespace networking;

// from https://stepik.org/lesson/12579/step/1?unit=3005
static int set_nonblock (SocketType fd)
{
#ifdef _WIN32
    ULONG flags;
#elif __linux__
    int flags;
#endif
#if defined(O_NONBLOCK)
    ﻿if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        ﻿flags = 0;
    ﻿return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
#ifdef _WIN32
    return ioctlsocket (fd, FIONBIO, & flags);
#elif __linux__
    return ioctl(fd, FIONBIO, &flags);
#endif
#endif
} 

static void ErrorProcessing (int theLine, const char* theFile)
{
#ifdef _WIN32
    std::cerr << "Error! | " 
                << "LINE : " << theLine << " : FILE : "  << theFile 
                << "Error Code : " << WSAGetLastError();
#endif
}

internal::TcpSocketImpl::TcpSocketImpl (SocketType theSocket) : AbstractSocketImpl (theSocket)
{
    set_nonblock (theSocket);
};

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
#ifdef _WIN32
    if (mySocket == INVALID_SOCKET) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
#endif
    set_nonblock (mySocket);
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
    if (aBindReturn != 0) {
        ErrorProcessing (__LINE__, __FILE__);
    }
    return aBindReturn;
}

bool internal::TcpSocketImpl::Listen()
{
#ifdef _WIN32
    if (listen (mySocket, SOMAXCONN) == SOCKET_ERROR) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
#elif __linux__
    if (listen (mySocket, SOMAXCONN) == -1) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
#endif
    return true;
}

SocketType internal::TcpSocketImpl::Accept()
{
    const auto& aClientSocket = accept (mySocket, nullptr, nullptr);
#ifdef _WIN32
    if (aClientSocket == INVALID_SOCKET) {
        ErrorProcessing (__LINE__, __FILE__);
    }
#endif
    set_nonblock (aClientSocket);
    return aClientSocket;
}

SocketType internal::TcpSocketImpl::Accept (sockaddr_in& theServerInfo)
{
#ifdef _WIN32
    int aSenderInfoSize = sizeof (theServerInfo);
#elif __linux__
    socklen_t aSenderInfoSize = sizeof (theServerInfo);
#endif
    const auto& aClientSocket = accept (mySocket, (sockaddr*) &theServerInfo, &aSenderInfoSize);
#ifdef _WIN32
    if (aClientSocket == INVALID_SOCKET) {
        ErrorProcessing (__LINE__, __FILE__);
    }
#endif
    set_nonblock (aClientSocket);
    return aClientSocket;
}

bool internal::TcpSocketImpl::Connect (const sockaddr_in& theServerInfo)
{
    int aConnectReturn = connect (mySocket, (sockaddr*) &theServerInfo, sizeof (theServerInfo));
#ifdef _WIN32
    if (aConnectReturn == SOCKET_ERROR) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
#endif
    return true;
}

void internal::TcpSocketImpl::Close()
{
#ifdef _WIN32
    int aSDReturn = shutdown (mySocket, SD_BOTH);
#elif __linux__
    int aSDReturn = shutdown (mySocket, SHUT_RDWR);
#endif

#ifdef _WIN32
    if (aSDReturn == SOCKET_ERROR) {
        ErrorProcessing (__LINE__, __FILE__);
    }
#endif

#ifdef _WIN32
    int aCSReturn = closesocket (mySocket);
#elif __linux__

    int aCSReturn = close (mySocket);
#endif

#ifdef _WIN32
    if (aCSReturn == SOCKET_ERROR) {
        ErrorProcessing (__LINE__, __FILE__);
    }
#endif
}

int internal::TcpSocketImpl::Read (char* theData, std::uint64_t theMaxDataSize)
{
    int aRecvReturn = recv (mySocket, theData, static_cast <int> (theMaxDataSize), 0);
#ifdef _WIN32
    if (aRecvReturn == SOCKET_ERROR) {
        ErrorProcessing(__LINE__, __FILE__);
        if (aRecvReturn == WSAENETDOWN
            || aRecvReturn == WSAENOTCONN
            || aRecvReturn == WSAENOTSOCK
            || aRecvReturn == WSAETIMEDOUT) {
        }
        return false;
    }
#elif __linux__
    if (aRecvReturn == 0 && errno != EAGAIN) {
        std::cout << "Shoot close()" << std::endl;
    }
#endif
    return aRecvReturn;
}

bool internal::TcpSocketImpl::Write (const char* theData, std::uint64_t theDataSize)
{
    int Retnd = send (mySocket, theData, static_cast<int> (theDataSize), 0);
#ifdef _WIN32
    if (Retnd == SOCKET_ERROR) {
        ErrorProcessing(__LINE__, __FILE__);
        return false;
    }
#endif
    return true;
}

TcpSocket::TcpSocket (SocketType theSocket) : 
    AbstractSocket (std::make_shared <internal::TcpSocketImpl> (theSocket))
{};

bool TcpSocket::Bind (const char* theReciverAddress, std::uint16_t thePort)
{
    return internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Bind (theReciverAddress, thePort);
}

bool TcpSocket::Listen()
{
    return internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Listen();
}

std::shared_ptr<TcpSocket> TcpSocket::Accept()
{
    const auto& aClientSocket = internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Accept();
    return std::make_shared <TcpSocket> (aClientSocket);
}

std::shared_ptr<TcpSocket> TcpSocket::Accept (sockaddr_in& theSenderInfo)
{
    const auto& aClientSocket =  internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Accept (theSenderInfo);
    return std::make_shared<TcpSocket> (aClientSocket);
}

bool TcpSocket::Connect (const sockaddr_in& theServerInfo)
{
    return  internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Connect (theServerInfo);
}

int TcpSocket::Read (char* theData, std::uint64_t theMaxDataSize)
{
    return internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Read (theData, theMaxDataSize);
}

bool TcpSocket::Write (const char* theData, std::uint64_t theDataSize)
{
    return internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Write (theData, theDataSize);
}

void TcpSocket::Close()
{
     internal::ToImpl<internal::TcpSocketImpl> (myImpl)->Close();
}

