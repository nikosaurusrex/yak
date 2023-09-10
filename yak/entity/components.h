#ifndef COMPONENTS_H
#define COMPONENTS_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

#include "gfx/mesh.h"
#include "gfx/texture.h"

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

#endif
