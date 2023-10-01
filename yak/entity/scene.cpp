#include "scene.h"

Assets::Assets(string assets_path)
    : assets_path(assets_path) {
}

Texture *Assets::load_texture(string path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

	log_info("Load Texture '%s'", path.c_str());

    Texture *texture = new Texture(path, GL_RGBA);
    textures.insert({path, texture});
    return texture;
}

Texture *Assets::get_texture(string path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }
    
    return 0;
}

Script *Assets::load_script(string path) {
    auto it = scripts.find(path);
    if (it != scripts.end()) {
        return it->second;
    }

	log_info("Load Script '%s'", path.c_str());

	Script *script = new Script(path);
    scripts.insert({path, script});
	script->load();
    return script;
}

Script *Assets::get_script(string path) {
    auto it = scripts.find(path);
    if (it != scripts.end()) {
        return it->second;
    }
    
    return 0;
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
