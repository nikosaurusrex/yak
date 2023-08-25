#ifndef EVENT_H
#define EVENT_H

#include "yakpch.h"

struct Event {

    enum EventType {
        RESIZE,
    };

    EventType type;
    s32 width;
    s32 height;
};

struct EventHandler {
    virtual void handle_event(Event event) = 0;
};

#endif