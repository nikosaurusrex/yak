#ifndef MESH_H
#define MESH_H

#include "yakpch.h"

struct Mesh {
    GLuint vao;
    GLuint vbos[3];
    u32 indices_count;

    Mesh(f32 *vertices, u32 vertices_count, f32 *tex_coords, u32 tex_coords_count, u32 *indices, u32 indices_count);
    ~Mesh();

    void bind();
    void render();
    void free();
};

struct Meshes {
    static Mesh *quad;

    static void init();
    static void free();
};

#endif