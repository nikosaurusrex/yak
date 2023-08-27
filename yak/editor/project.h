#ifndef PROJECT_H
#define PROJECT_H

#include "yakpch.h"

#include "entity/scene.h"

struct Project {
    Scene *scene;
    Assets *assets;

    Project(Scene *scene, Assets *assets);
    ~Project();
};

#endif
