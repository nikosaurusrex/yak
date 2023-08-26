#ifndef SCENE_H
#define SCENE_H

#include "entity/ecs.h"

struct Scene {
    EntityRegistry registry;
    map<EntityId, Entity> entity_map;

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
