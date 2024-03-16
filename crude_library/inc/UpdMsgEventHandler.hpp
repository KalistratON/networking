#ifndef UPDMSGEVENTHANDLER_HPP
#define UPDMSGEVENTHANDLER_HPP

#include "../../event_system/inc/AbstractEvent.hpp"
#include "../../event_system/inc/AbstractEventHandler.hpp"
#include "../inc/UpdSocket.hpp"

#include <memory>

namespace networking {

class UpdMsgEventHandler : public event_system::AbstractEventHandler {

public:
    UpdMsgEventHandler ();
    virtual void call (const event_system::AbstractEvent* theEvent) const override;

private:
    std::shared_ptr<UpdSocket> myUpdSocket;
};

}

#endif