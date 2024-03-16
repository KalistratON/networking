#ifndef UPDMSGEVENT_HPP
#define UPDMSGEVENT_HPP

#include "../../event_system/inc/AbstractEvent.hpp"
#include "../inc/UpdSocket.hpp"

#include <memory>

namespace networking {

class UpdMsgEvent : public event_system::AbstractEvent {

public:
    UpdMsgEvent();
    UpdMsgEvent (std::shared_ptr<UpdSocket> theUpdSocket);
    const std::shared_ptr<UpdSocket>& get() const;

private:
    std::shared_ptr<UpdSocket> myUpdSocket;
};

}

#endif