#ifndef PANELS_H
#define PANELS_H

#include "imgui.h"

#include "entity/ecs.h"
#include "entity/scene.h"

struct Framebuffer;
struct Engine;
struct SceneView {
    Engine *engine;
    Entity *selection;
    Framebuffer *framebuffer;
    ImVec2 min_region;
    ImVec2 max_region;
    ImVec2 offset;
    s32 width;
    s32 height;
    bool need_resize = true;

    SceneView(Engine *engine, Entity *selection);
    ~SceneView();

    void init();
    void render(Scene *scene);

    void resize();

    void on_mouse_button(s32 button, s32 action);
};

struct SceneHierarchy {
    Entity *selection;

    SceneHierarchy(Entity *selection);

    void render(Scene *scene);
};

struct Assets;
struct PropertiesPanel {
    Assets *assets;

    PropertiesPanel(Assets *assets);

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

#endif
