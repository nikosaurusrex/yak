#ifndef PANELS_H
#define PANELS_H

#include "entity/ecs.h"
#include "entity/scene.h"

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
