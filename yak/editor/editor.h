#ifndef GUI_H
#define GUI_H

#include "yakpch.h"

#include "core/event.h"
#include "editor/panels.h"
#include "entity/scene.h"

enum mode {
    MODE_EDIT = 0,
    MODE_PLAY = 1
};

using EntityId = u64;
struct RendererImGui {
    static void init(GLFWwindow *window);
    static void deinit();

    static void begin();
    static void end(s32 width, s32 height);
};

struct Window;
struct Engine;
struct Framebuffer;
struct Project;
struct Camera;
struct Editor : public EventHandler {
    Engine *engine;
    Window *window;
    Project *project;
    SceneView *scene_view;
    SceneHierarchy *scene_hierarchy;
    PropertiesPanel *properties_panel;
    ContentBrowser *content_browser;
    RenderStatsPanel *render_stats_panel;
    Toolbar *toolbar;
    Camera *camera;
    Entity selection;

    u64 mode = MODE_EDIT;

    Editor(Window *window, Project *project);
    ~Editor();

    void init();
    void run();

    void render();
    void render_menu();

    virtual void handle_event(Event event) override;
};

void run_editor();

#endif
