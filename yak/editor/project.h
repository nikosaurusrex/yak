#ifndef PROJECT_H
#define PROJECT_H

#include "yakpch.h"

#include <fstream>

#include "entity/scene.h"

struct Project {
    string path;
    string name;
    Assets *assets;

    Project(string path);
    Project(string path, string name);
    ~Project();

    void load();
    void save();
};

Scene *load_scene_file(string path, Assets *assets);
void save_scene_file(Scene *scene, Assets *assets);

#endif
