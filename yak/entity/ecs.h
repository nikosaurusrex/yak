#ifndef ECS_H
#define ECS_H

#include "yakpch.h"
#include "gfx/mesh.h"
#include "math/ymath.h"

using EntityId = u64;

struct EntityRegistry {
    u64 entity_count = 0;

    map<std::type_index, map<EntityId, std::any>> components;

    EntityId create() {
        return entity_count++;
    }

    template <typename T>
    void add(EntityId id, T &&component) {
        components[std::type_index(typeid(T))][id] = component;
    }

    template <typename T>
    bool has(EntityId id) {
        return components[std::type_index(typeid(T))].contains(id); 
    }

    template <typename T>
    T get(EntityId id) {
        auto comp = components[std::type_index(typeid(T))][id];
        return std::any_cast<T>(comp);
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
    T get() {
        return registry->get<T>(id);
    }
};

struct Scene {
    EntityRegistry registry;
    map<EntityId, Entity> entity_map;

    Entity create_entity() {
        EntityId id = registry.create();
        Entity entity = {&this->registry, id};
        entity_map.insert({id, entity});
        return entity;
    }

    template <typename T>
    array<Entity> get_entities_with_component() {
        auto components = registry.components[std::type_index(typeid(T))];

        array<Entity> entities;
        entities.reserve(components.size());

        for(auto kv : components) {
            Entity entity = entity_map.at(kv.first);

            entities.push_back(entity);
        }

        return entities;
    }
};

#endif