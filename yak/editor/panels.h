#ifndef PANELS_H
#define PANELS_H

#include "imgui.h"

#include <filesystem>

#include "entity/ecs.h"
#include "entity/scene.h"
#include "gfx/texture.h"

namespace fs = std::filesystem;

struct Camera;
struct Editor;
struct Framebuffer;
struct Engine;
struct SceneView {
    Editor *editor;
    Entity *selection;
    Framebuffer *framebuffer;
    ImVec2 offset;
    ImVec2 region;
    ImVec2 last_region;
    bool need_resize = true;
    s32 guizmo_operation = 0;

    glm::mat4 proj_mat;
    glm::mat4 view_mat;

    SceneView(Editor *editor, Entity *selection);
    ~SceneView();

    void init();
    void render(Camera *camera);

    void resize();

    void on_mouse_button(s32 button, s32 action);
    void on_key_up(s32 key);
};

struct SceneHierarchy {
    Entity *selection;

    SceneHierarchy(Entity *selection);

    void render(Scene *scene);
};

struct Assets;
struct PropertiesPanel {
    Editor *editor;
    Assets *assets;

    PropertiesPanel(Editor *editor, Assets *assets);

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

struct Toolbar {
    void render(Editor *editor);
};

struct ContentBrowser {
    Editor *editor;
    Texture *img_file;
    Texture *img_folder;
    Texture *img_script;
    fs::path project_path;
    fs::path path;

    ContentBrowser(Editor *editor, string project_path);
    ~ContentBrowser();

    void init();
    void render();

    void item_popup(string &text_input_mode, string &text_input_context, bool &open_text_input, fs::path &file_name);
};

struct RenderStatsPanel {
    void render();
};

struct DebugConsole {
    static void render();
    static void append();
};

#endif
