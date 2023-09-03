#ifndef PANELS_H
#define PANELS_H

#include "imgui.h"

#include "entity/ecs.h"
#include "entity/scene.h"

struct Camera;
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
    void render(Camera *camera);

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

    template <typename T>
    void render_add_component(Entity entity, string name);

    template <typename T, typename Callback>
    void render_component(Entity entity, string name, Callback callback);

    template <typename Callback>
    bool render_property(const char *name, bool has_remove_button, Callback callback);

    void render_vec_xyz(glm::vec3 *vec, const char *label, const char *id);
};

struct Editor;
struct Toolbar {
    void render(Editor *editor);
};

struct ContentBrowser {
    void render();
};

struct RenderStatsPanel {
    void render();
};

struct DebugConsole {
    static void render();
};

#endif
