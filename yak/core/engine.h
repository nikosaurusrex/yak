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
    Scene *scene;
    Assets *assets;
    Renderer2D *renderer_2d;
    bool running = false;
    Layers layers;

    Engine(Window *window);
    Engine(Window *window, Scene *scene, Assets *assets);
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
