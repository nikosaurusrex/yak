#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>

#include "gfx/mesh.h"
#include "gfx/texture.h"

struct TransformComponent {
    glm::vec3 translation;
    glm::vec3 scale;

    TransformComponent(glm::vec3 translation, glm::vec3 scale)
    : translation(translation), scale(scale) {}
};

struct MeshComponent {
    Mesh *mesh;

    MeshComponent(Mesh *mesh) : mesh(mesh) {};
};

struct TextureComponent {
    Texture *texture;

    TextureComponent(Texture *texture) : texture(texture) {};
};

#endif
