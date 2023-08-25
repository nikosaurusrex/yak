#ifndef GUI_H
#define GUI_H

#include "core/event.h"

using EntityId = u64;
struct Window;
struct RendererImGui {
    Window *window;

    RendererImGui(Window *window);
    ~RendererImGui();

    void init();

    void begin();
    void end();
};

struct Engine;
struct Framebuffer;
struct Editor : public EventHandler {
    Engine *engine;
    Window *window;
    RendererImGui *renderer;
    Framebuffer *framebuffer;
    EntityId selection;

    Editor(Window *window);
    ~Editor();

    void init();
    void run();

    void render();
    void render_menu();
    void render_scene();
    void render_scene_hierarchy();

    virtual void handle_event(Event event) override;
};

void run_editor();

#endif
