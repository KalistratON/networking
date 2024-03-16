#include "../inc/UpdSocket.hpp"
#include "../inc/UpdSocketImpl.hpp"
#include "../inc/Helper.hpp"

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#elif _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
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

internal::UpdSocketImpl::UpdSocketImpl(SocketType theSocket) :
    AbstractSocketImpl(theSocket)
{};

bool internal::UpdSocketImpl::Bind (const char* theReciverAddress, std::uint16_t thePort)
{
#ifdef _WIN32
    WSADATA wsData;
    int anError = WSAStartup (MAKEWORD (2, 2), &wsData);
    if (anError) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
#endif
    mySocket = socket (AF_INET, SOCK_DGRAM, 0);
#ifdef _WIN32
    if (mySocket == INVALID_SOCKET) {
        ErrorProcessing (__LINE__, __FILE__);
        return false;
    }
#endif
    sockaddr_in aSocketInfo;
    in_addr ip_to_num;
    inet_pton (AF_INET, theReciverAddress, &ip_to_num);

    aSocketInfo.sin_family = AF_INET;
    aSocketInfo.sin_addr = ip_to_num;       // INADDR_ANY
    aSocketInfo.sin_port = htons (thePort);

    int aBindReturn = bind (mySocket, reinterpret_cast <const sockaddr*> (&aSocketInfo), sizeof (aSocketInfo));
    if (aBindReturn != 0) {
        ErrorProcessing (__LINE__, __FILE__);
    }
    return aBindReturn;
}

bool internal::UpdSocketImpl::ReadDatagram (char* theData, std::uint64_t theMaxDataSize)
{
    int aRecvReturn = recvfrom (mySocket, theData, static_cast <int> (theMaxDataSize), 0, 
                                nullptr, nullptr);
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
#endif
    return true;
}

bool internal::UpdSocketImpl::ReadDatagram (char* theData, std::uint64_t theMaxDataSize, 
                                            sockaddr_in& theSenderInfo)
{
    int aSenderInfoSize = sizeof (theSenderInfo);
    int aRecvReturn = recvfrom (mySocket, theData, static_cast <int> (theMaxDataSize), 0,
                                (sockaddr*) (&theSenderInfo), &aSenderInfoSize);
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
#endif
    return true;
}

bool internal::UpdSocketImpl::WriteDatagram (const char* theData, std::uint64_t theDataSize, 
                                             const char* theReciverAddress, std::uint16_t thePort)
{
    sockaddr_in aSocketInfo;
    in_addr ip_to_num;
    inet_pton (AF_INET, theReciverAddress, &ip_to_num);

    aSocketInfo.sin_family = AF_INET;
    aSocketInfo.sin_addr = ip_to_num;
    aSocketInfo.sin_port = htons (thePort);

    int aRecvReturn = sendto (mySocket, theData, static_cast <int> (theDataSize), 0,
                              reinterpret_cast <sockaddr*> (&aSocketInfo), sizeof (aSocketInfo));
#ifdef _WIN32
    if (aRecvReturn == SOCKET_ERROR) {
        ErrorProcessing(__LINE__, __FILE__);
        if (aRecvReturn == WSAENETDOWN
            || aRecvReturn == WSAENOTCONN
            || aRecvReturn == WSAENOTSOCK
            || aRecvReturn == WSAETIMEDOUT) {
            return false;
        }
    }
#endif
    return true;
}

UpdSocket::UpdSocket (SocketType theSocket) :
    AbstractSocket (std::make_shared <internal::UpdSocketImpl> (theSocket))
{};

bool UpdSocket::Bind (const char* theReciverAddress, std::uint16_t thePort)
{
    return internal::ToImpl<internal::UpdSocketImpl> (myImpl)->Bind (theReciverAddress, thePort);
}

bool UpdSocket::ReadDatagram (char* theData, std::uint64_t theMaxDataSize)
{
    return internal::ToImpl<internal::UpdSocketImpl> (myImpl)->ReadDatagram (theData, theMaxDataSize);
}

bool UpdSocket::ReadDatagram (char* theData, std::uint64_t theMaxDataSize, sockaddr_in& theSenderInfo)
{
    return internal::ToImpl<internal::UpdSocketImpl> (myImpl)->ReadDatagram (theData, theMaxDataSize, theSenderInfo);
}

bool UpdSocket::WriteDatagram (const char* theData, std::uint64_t theDataSize, 
                               const char* theReciverAddress, std::uint16_t thePort)
{
    return internal::ToImpl<internal::UpdSocketImpl> (myImpl)->WriteDatagram (theData, theDataSize, 
                                                                              theReciverAddress, thePort);
}

