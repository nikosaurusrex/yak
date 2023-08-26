#ifndef ECS_H
#define ECS_H

#include "yakpch.h"

#include "entity/components.h"
#include "gfx/mesh.h"

using EntityId = u64;

struct EntityRegistry {
    u64 entity_id_counter = 1;

    map<EntityId, map<std::type_index, std::any>> components;

    EntityId create() {
        return entity_id_counter++;
    }

    template <typename T>
    void add(EntityId id, T &&component) {
        components[id][std::type_index(typeid(T))] = component;
    }

    void remove(EntityId id) {
        components.erase(id);
    }

    template <typename T>
    bool has(EntityId id) {
        return components[id].contains(std::type_index(typeid(T)));
    }

    template <typename T>
    T &get(EntityId id) {
        return std::any_cast<T &>(components[id][std::type_index(typeid(T))]);
    }
};

struct Entity {
    EntityRegistry *registry;
    EntityId id;

    Entity() = default;
    Entity(EntityRegistry *registry, EntityId id) : registry(registry), id(id) {}

    template <typename T>
    void add(T &&component) {
        registry->add<T>(id, std::forward<T>(component));
    }

    template <typename T>
    bool has() {
        return registry->has<T>(id);
    }

    template <typename T>
    T &get() {
        return registry->get<T>(id);
    }
};

struct Scene {
    EntityRegistry registry;
    map<EntityId, Entity> entity_map;

    Entity create_entity(string tag) {
        EntityId id = registry.create();
        Entity entity = {&this->registry, id};

        entity.add<TagComponent>(tag);

        entity_map.insert({id, entity});
        return entity;
    }

    void destroy_entity(EntityId id) {
        entity_map.erase(id);
        registry.remove(id);
    }

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

    array<Entity> get_entities() {
        array<Entity> entities;

        for (auto kv : registry.components) {
            entities.push_back(entity_map.at(kv.first));
        }

        return entities;
    }

    Entity from_id(EntityId id) {
        return entity_map.at(id);
    }
};

#endif
