#ifndef ENGINE_H
#define ENGINE_H

#include "core/event.h"
#include "core/layer.h"

struct Scene;
struct Window;
struct Renderer2D;
struct RendererImGui;
struct Engine : public EventHandler {
    Window *window = 0;
    Scene *scene;
    Renderer2D *renderer_2d;
    bool running = false;
    Layers layers;

    Engine(Window *window);
    ~Engine();

    void init();
    void run();
    void loop();
    void update();
    void render();
    void stop();
    
    virtual void handle_event(Event event) override;
};

#endif
