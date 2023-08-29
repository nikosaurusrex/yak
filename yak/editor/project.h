#ifndef PROJECT_H
#define PROJECT_H

#include "yakpch.h"

#include <fstream>

#include "entity/scene.h"

struct Project;
struct ProjectFile {
    string path;
    Project *project;

    ProjectFile(string path, Project *project);

    void read();
    glm::vec3 read_vec3(std::ifstream &inf);
    glm::vec4 read_vec4(std::ifstream &inf);

    void write();
    void write(std::ofstream &of, glm::vec3 vec);
    void write(std::ofstream &of, glm::vec4 vec);
};

struct Project {
    string path;
    string name;
    Scene *scene;
    Assets *assets;

    Project(string path);
    Project(string path, string name);
    ~Project();

    void load();
    void save();
};

#endif
