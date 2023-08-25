#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "gfx/mesh.h"
#include "gfx/texture.h"
#include "math/ymath.h"

struct TransformComponent {
    mat4 transform;

    TransformComponent(mat4 transform) : transform(transform) {}
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