#include "project.h"

void Assets::load_texture(string path) {
    string p = assets_path + path;

    Texture *texture = new Texture(p, GL_RGBA);
    textures.insert({path, texture});
    return texture;
}

Project::Project(Scene *scene) : scene(scene) {
}

Project::~Project() {
}
