#ifndef PROJECT_H
#define PROJECT_H

#include "yakpch.h"

#include <fstream>

#include "entity/scene.h"

struct SceneFile {
    string path;
    Scene *scene;
    Assets *assets;

    SceneFile(string path, Scene *scene, Assets *assets);

    void read();
    glm::vec3 read_vec3(std::ifstream &inf);
    glm::vec4 read_vec4(std::ifstream &inf);

    void write();
    void write(std::ofstream &of, glm::vec3 vec);
    void write(std::ofstream &of, glm::vec4 vec);
};

struct Project;
struct ProjectFile {
    string path;
    Project *project;

    ProjectFile(string path, Project *project);

    void read();
    void write();
};

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
