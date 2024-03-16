#include "../inc/AbstractSocket.hpp"
#include "../inc/AbstractSocketImpl.hpp"

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

internal::AbstractSocketImpl::AbstractSocketImpl (SocketType theSocket) :
    mySocket (theSocket)
{};

SocketType internal::AbstractSocketImpl::GetSocket() const
{
    return mySocket;
};

AbstractSocket::AbstractSocket (const ImplPtr& theImplPtr) :
    myImpl (theImplPtr)
{};

SocketType AbstractSocket::GetSocket() const
{
    return myImpl->GetSocket();
};

AbstractSocket::operator bool() const
{
    return GetSocket() != -1;
}

bool AbstractSocket::operator == (const AbstractSocket& theCmp) const
{
    return GetSocket() == theCmp.GetSocket();
}

bool AbstractSocket::operator != (const AbstractSocket& theCmp) const
{
    return GetSocket() != theCmp.GetSocket();
}