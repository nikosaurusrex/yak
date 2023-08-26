#ifndef COMPONENTS_H
#define COMPONENTS_H

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
    glm::vec3 scale;

    TransformComponent() {
        translation = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
    }

    TransformComponent(glm::vec3 translation, glm::vec3 scale)
    : translation(translation), scale(scale) {}
};

struct MeshComponent {
    Mesh *mesh;

    MeshComponent() {
        mesh = Meshes::quad;
    }

    MeshComponent(Mesh *mesh) : mesh(mesh) {};
};

struct TextureComponent {
    Texture *texture;

    TextureComponent() {
        texture = 0;
    }

    TextureComponent(Texture *texture) : texture(texture) {};
};

#endif
