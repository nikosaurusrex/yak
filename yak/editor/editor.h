#ifndef GUI_H
#define GUI_H

#include "core/event.h"
#include "entity/ecs.h"
#include "entity/scene.h"

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

struct SceneHierarchy {
    Entity *selection;

    SceneHierarchy(Entity *selection);

    void render(Scene *scene);
};

struct PropertiesPanel {
    void render(Scene *scene, Entity entity);

    void render_components(Entity entity);

    template <typename T, typename Callback>
    void render_component(Entity entity, string name, Callback callback);

    template <typename T>
    void render_add_component(Entity entity, string name);
};

struct ContentBrowser {
    void render();
};

struct Editor : public EventHandler {
    Engine *engine;
    Window *window;
    RendererImGui *renderer;
    Framebuffer *framebuffer;
    SceneHierarchy *scene_hierarchy;
    PropertiesPanel *properties_panel;
    ContentBrowser *content_browser;
    Entity selection;

    Editor(Window *window);
    ~Editor();

    void init();
    void run();

    void render();
    void render_menu();
    void render_scene();

    bool property_begin_component();
    void property_end_component(bool open);

    virtual void handle_event(Event event) override;
};

void run_editor();

#endif
