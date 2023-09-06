#include "scene.h"

Assets::Assets(string assets_path)
    : assets_path(assets_path) {
}

Texture *Assets::load_texture(string path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

    string p = assets_path + path;

    Texture *texture = new Texture(p, path, GL_RGBA);
    textures.insert({path, texture});
    return texture;
}

Scene::Scene(string path)
    : path(path) {
}

Entity Scene::create_entity(string tag) {
    EntityId id = registry.create();
    Entity entity = {&this->registry, id};

    entity.add<TagComponent>(tag);

    entity_map.insert({id, entity});
    return entity;
}

void Scene::destroy_entity(EntityId id) {
    entity_map.erase(id);
    registry.erase(id);
}

array<Entity> Scene::get_entities() {
    array<Entity> entities;

    for (auto kv : registry.components) {
        entities.push_back(entity_map.at(kv.first));
    }

    return entities;
}

Entity Scene::from_id(EntityId id) {
    return entity_map.at(id);
}
