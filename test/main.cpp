#include "../crude_library/inc/UpdSocket.hpp"
#include "../crude_library/inc/TcpServer.hpp"

#include <iostream>

using namespace networking;

int main()
{
    TcpServer aTcpServer ("127.0.0.1", 1234);
    aTcpServer.Listen();
    const auto& aTrcSocket = aTcpServer.NextPendingConnection();
    return 0;
}