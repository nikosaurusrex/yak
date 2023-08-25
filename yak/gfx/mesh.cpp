#include "mesh.h"

Mesh::Mesh(f32 *vertices, u32 vertices_count, f32 *tex_coords, u32 tex_coords_count, u32 *indices, u32 indices_count)
    : indices_count(indices_count) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(f32), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, tex_coords_count * sizeof(f32), tex_coords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indices_count, indices, GL_STATIC_DRAW);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbos);
}

void Mesh::bind() {
    glBindVertexArray(vao);
}

void Mesh::render() {
    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
}

Mesh *Meshes::quad = 0;

void Meshes::init() {
    f32 quad_vertices[] = {1.0f, 1.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f};


    f32 quad_tex_coords[] = {1.0f, 0.0f,
                            1.0f, 1.0f,
                            0.0f, 1.0f,
                            0.0f, 0.0f};

    u32 quad_indices[] = {0, 1, 3,
                        1, 2, 3};

    Meshes::quad = new Mesh(&quad_vertices[0], 12, &quad_tex_coords[0], 8, &quad_indices[0], 6);
}

void Meshes::free() {
    delete Meshes::quad;
}