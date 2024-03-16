/*! Define mechanism of invoking event dispatchers.
    Workflow protocol:
        1. create inheritor of AbstractEventHandler
        2. subscribed by appropriate event AbstractEvent::operator +=
*/