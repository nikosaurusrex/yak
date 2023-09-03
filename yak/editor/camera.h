#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "yakpch.h"

#include "glm/glm.hpp"

struct Camera {
    glm::mat4 projection;
    glm::mat4 view;

    f32 fov;
    f32 aspect;
    f32 near;
    f32 far;

    f32 yaw;
    f32 pitch;

    glm::vec3 focal_point;
    f32 distance;

    Camera() = default;
    Camera(f32 fov, f32 aspect, f32 near, f32 far);

    void resize(s32 width, s32 height);
};

#endif
