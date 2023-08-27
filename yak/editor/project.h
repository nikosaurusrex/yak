#ifndef PROJECT_H
#define PROJECT_H

#include "yakpch.h"

#include "entity/scene.h"
#include "gfx/texture.h"

struct Assets {
    string assets_path;
    map<string, Texture *> textures;

    Texture *load_texture(string path);
};

struct Project {
    Assets *assets;
    Scene *scene;

    Project(Scene *scene);
    ~Project();
};

#endif
