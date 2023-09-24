#ifndef EVENT_H
#define EVENT_H

#include "common.h"

struct Event {

    enum EventType {
        RESIZE,
        MOUSE_BUTTON,
        MOUSE_MOVE,
        KEY
    };

    EventType type;

    union {
        struct {
            s32 width;
            s32 height;
        };
        struct {
            s32 button;
            s32 action;
            s32 mods;
        };
        struct {
            f64 xpos;
            f64 ypos;
        };
    };
};

struct EventHandler {
    virtual void handle_event(Event event) = 0;
};

#endif
