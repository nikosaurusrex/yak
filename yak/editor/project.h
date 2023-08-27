#ifndef PROJECT_H
#define PROJECT_H

#include "yakpch.h"

#include "entity/scene.h"

struct Project {
    string path;
    string name;
    Scene *scene;
    Assets *assets;

    Project(string path);
    Project(string path, string name);
    ~Project();

    void load();
    void load_project_file(string path);
    void create_project_file(string path);
};

#endif
