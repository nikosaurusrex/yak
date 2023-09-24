#ifndef SCENE_H
#define SCENE_H

#include "common.h"

#include "entity/ecs.h"
#include "gfx/texture.h"
#include "script/script.h"

struct Assets {
    string assets_path;
    map<string, Texture *> textures;
    map<string, Script *> scripts;

    Assets(string assets_path);

    Texture *load_texture(string path);
    Texture *get_texture(string path);

    Script *load_script(string path);
    Script *get_script(string path);
};

struct Scene {
    EntityRegistry registry;
    map<EntityId, Entity> entity_map;
    string path;

    Scene(string path);

    Entity create_entity(string tag);

    void destroy_entity(EntityId id);

    template <typename T>
    array<Entity> get_entities_with_component() {
        array<Entity> entities;

        for (auto kv : registry.components) {
            EntityId entity = kv.first;
            
            if (registry.components[entity].contains(std::type_index(typeid(T)))) {
                entities.push_back(entity_map.at(entity));
            }
        }

        return entities;
    }

    array<Entity> get_entities();

    Entity from_id(EntityId id);
};

#endif
