#ifndef ENGINE_H
#define ENGINE_H

#include "core/layer.h"
#include "core/event.h"
#include "entity/scene.h"

struct Assets;
struct Scene;
struct Window;
struct Renderer2D;
struct RendererImGui;
struct Engine : public EventHandler {
    Window *window = 0;
    Scene *scene = 0;
    Assets *assets;
    bool running = false;
    Layers layers;

    f64 fps_time_last = 0;
    u64 frames = 0;

    Engine(Window *window);
    Engine(Window *window, Assets *assets);
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
