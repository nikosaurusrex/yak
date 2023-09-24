#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

struct EventHandler;
struct Window {
    GLFWwindow *handle = 0;
    const char *title;
    s32 width;
    s32 height;

    Window(const char *title, s32 width, s32 height);

    void init();
    void create();
    bool update();
    void expand();
    void destroy();

    void set_event_handler(EventHandler *handler);
};

#endif
