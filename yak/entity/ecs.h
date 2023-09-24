#ifndef ECS_H
#define ECS_H

#include "common.h"

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

    template <typename T>
    void remove(EntityId id) {
        components[id].erase(std::type_index(typeid(T)));
    }

    void erase(EntityId id) {
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
    EntityId id = 0;

    Entity() = default;
    Entity(EntityRegistry *registry, EntityId id) : registry(registry), id(id) {}

    template <typename T>
    void add(T &&component) {
        registry->add<T>(id, std::forward<T>(component));
    }

    template <typename T>
    void remove() {
        registry->remove<T>(id);
    }

    template <typename T>
    bool has() {
        return registry->has<T>(id);
    }

    template <typename T>
    T &get() {
        return registry->get<T>(id);
    }
    
    operator bool() { return id != 0; }
    operator EntityId() { return id; }

    bool operator==(Entity other) {
        return id == other.id;
    }
};

#endif
