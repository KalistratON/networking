
#include "../event_system/inc/AbstractEventHandler.hpp"
#include "../event_system/inc/AbstractEventHandler.hpp"

#include "../crude_library/inc/TcpConnectionEvent.hpp"
#include "../crude_library/inc/TcpConnectionEventHandler.hpp"
#include "../crude_library/inc/TcpMsgEvent.hpp"
#include "../crude_library/inc/TcpMsgEventHandler.hpp"
#include "../crude_library/inc/UpdMsgEvent.hpp"
#include "../crude_library/inc/UpdMsgEventHandler.hpp"

#include "../crude_library/inc/UpdSocket.hpp"
#include "../crude_library/inc/TcpServer.hpp"

#include <iostream>

using namespace networking;
using namespace event_system;

namespace test_classes {

class CustomTcpConnectionHandle : public TcpConnectionEventHandler
{
    void call (const AbstractEvent* theEvent) const override
    {
        const auto anEvent = static_cast<const TcpConnectionEvent*> (theEvent);
        if (!anEvent) {
            return;
        }

        const auto& aSocket = anEvent->get();
        std::cout << "CustomTcpConnectionHandle | New connection was revieved" << std::endl;
    }
};

class CustomTcpMsgHandle : public TcpMsgEventHandler
{
    void call (const AbstractEvent* theEvent) const override
    {
        const auto anEvent = static_cast<const TcpConnectionEvent*> (theEvent);
        if (!anEvent) {
            return;
        }

        const auto& aSocket = anEvent->get();
        std::cout << "CustomTcpMsgHandle | New message was revieved" << std::endl;

        const size_t aMaxSize = 100;
        char aData [aMaxSize];
        aSocket->Read (aData, aMaxSize);
        std::cout << "Message | " << aData << std::endl;
        std::cout << "Write back ..." << std::endl;
        bool aResWrite = aSocket->Write (aData, aMaxSize);
        if(aResWrite) {
            std::cout << "Sucesfull" << std::endl;
        }
        else {
            std::cout << "Unsucesfull!" << std::endl;
        }
    }
};

class CustomUpdMsgHandle : public UpdMsgEventHandler
{
    void call (const AbstractEvent* theEvent) const override
    {
        const auto anEvent = static_cast<const TcpConnectionEvent*> (theEvent);
        if (!anEvent) {
            return;
        }

        const auto& aSocket = anEvent->get();
        std::cout << "CustomTcpMsgHandle | New message was revieved" << std::endl;

        const size_t aMaxSize = 100;
        char aData [aMaxSize];
        aSocket->Read (aData, aMaxSize);
        std::cout << "Message | " << aData << std::endl;
        std::cout << "Write back ..." << std::endl;
        bool aResWrite = aSocket->Write (aData, aMaxSize);
        if(aResWrite) {
            std::cout << "Sucesfull" << std::endl;
        }
        else {
            std::cout << "Unsucesfull!" << std::endl;
        }
    }
};
}

int main()
{
    const char* anAddress = "127.0.0.1";
    size_t aPort = 1234;
    TcpServer aServer (anAddress, aPort);
    
    // launch endless process
    if(!aServer.Listen()) {
        return 1;
    }

    return 0;
}