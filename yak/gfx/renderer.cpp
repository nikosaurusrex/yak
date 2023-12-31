#include "renderer.h"

#include <glm/gtx/transform.hpp>

#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

u64 RenderStats::fps = 0;
f32 RenderStats::mspf = 0;
u64 RenderStats::render_calls = 0;
u64 RenderStats::vertices = 0;
u64 RenderStats::indices = 0;

void Renderer2D::begin() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderStats::render_calls = 0;
    RenderStats::vertices = 0;
    RenderStats::indices = 0;
}

void Renderer2D::render_quad() {
    Mesh *mesh = Meshes::quad;
    mesh->bind();
    mesh->render();

    RenderStats::render_calls++;
    RenderStats::vertices += 12;
    RenderStats::indices += 6;
}

void Renderer2D::end() {
    glDisable(GL_BLEND);
}

void Renderer2D::resize(s32 width, s32 height) {
    glViewport(0, 0, width, height);
}
