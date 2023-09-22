#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

#include "gfx/mesh.h"
#include "gfx/texture.h"
#include "script/script.h"

struct TagComponent {
    string tag;

    TagComponent(string tag) : tag(tag) {}
    TagComponent(const char *str) : tag(str) {}
};

struct TransformComponent {
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;

    TransformComponent() {
        translation = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
    }

    TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
    : translation(translation), rotation(rotation), scale(scale) {}

    glm::mat4 transform() {
        return glm::translate(glm::mat4(1.0f), translation) * 
                glm::toMat4(glm::quat(glm::radians(rotation))) *
                glm::scale(glm::mat4(1.0f), scale);
    }
};

struct RendererComponent {
    Mesh *mesh;
    Texture *texture;
    glm::vec4 color;

    RendererComponent() {
        mesh = Meshes::quad;
        texture = 0;
        color = glm::vec4(1.0f);
    }

    RendererComponent(Mesh *mesh, Texture *texture, glm::vec4 color)
        : mesh(mesh), texture(texture), color(color)
    {}
};

struct CameraComponent {
    glm::mat4 projection; 
    glm::mat4 view;
    
    glm::vec3 position;
    f32 rotation;

    bool is_main_camera = false;

    CameraComponent() {
        f32 x = 2.0f;
        f32 y = 2.0f / 16.0f * 9.0f;
        f32 z = 1.0f;
        projection = glm::ortho(-x, x, -y, y, -z, z);
        view = glm::mat4(1.0f);
        position = glm::vec3(0.0f);
        rotation = 0;
    }

    CameraComponent(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far) {
        projection = glm::ortho(left, right, bottom, top, near, far);
        position = glm::vec3(0.0f);
        rotation = 0;
        
        calculate_view_matrix();
    }

    void calculate_view_matrix() {
        auto transform = glm::translate(glm::mat4(1.0f), position)
                          * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

        view = glm::inverse(transform);
    }
};

struct ScriptComponent {
	Script *script;
};

#endif
