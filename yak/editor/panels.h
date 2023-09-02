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
    ImVec2 offset;
    ImVec2 region;
    ImVec2 last_region;
    bool need_resize = true;

    glm::mat4 proj_mat;
    glm::mat4 view_mat;

    SceneView(Engine *engine, Entity *selection);
    ~SceneView();

    void init();
    void render();

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

struct RenderStatsPanel {
    void render();
};

#endif
