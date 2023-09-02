#ifndef GUI_H
#define GUI_H

#include "core/event.h"
#include "editor/panels.h"
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
struct Project;
struct Editor : public EventHandler {
    Engine *engine;
    Window *window;
    Project *project;
    RendererImGui *renderer;
    SceneView *scene_view;
    SceneHierarchy *scene_hierarchy;
    PropertiesPanel *properties_panel;
    ContentBrowser *content_browser;
    RenderStatsPanel *render_stats_panel;
    Entity selection;

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
