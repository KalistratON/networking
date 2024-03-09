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

AbstractSocket::AbstractSocket (SocketType theSocket) :
    myImpl (std::make_shared<internal::AbstractSocketImpl>(theSocket))
{};