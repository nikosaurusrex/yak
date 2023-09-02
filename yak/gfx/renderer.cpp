#include "renderer.h"

#include <glm/gtx/transform.hpp>

#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

void Renderer2D::begin() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::render_quad() {
    Mesh *mesh = Meshes::quad;
    mesh->bind();
    mesh->render();
}

void Renderer2D::end() {
}

void Renderer2D::resize(s32 width, s32 height) {
    glViewport(0, 0, width, height);
    
    glm::mat4 proj_matrix = glm::ortho<f32>(0, width, 0, height, -1, 1);
    Shaders::load_for_all("proj_mat", proj_matrix);
}
