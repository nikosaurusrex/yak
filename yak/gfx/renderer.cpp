#include "renderer.h"

#include <glm/gtx/transform.hpp>

#include "entity/components.h"

Renderer2D::Renderer2D(Window *window) : window(window) {
}

Renderer2D::~Renderer2D() {
}

void Renderer2D::init() {
}

void Renderer2D::begin() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::render_scene(Scene *scene) {
    auto entities = scene->get_entities_with_component<RendererComponent>();

    for (auto entity : entities) {
        auto &rc = entity.get<RendererComponent>();

        Shader *shader;

        if (rc.texture) {
            shader = Shaders::texture;
            rc.texture->bind(0);
        } else {
            shader = Shaders::simple;
        }

        shader->use();
        shader->load_vec4("in_color", rc.color);

        if (entity.has<TransformComponent>()) {
            TransformComponent tc = entity.get<TransformComponent>();

            auto model_mat = glm::mat4(1.0);
            auto translation = tc.translation;
            auto scale = tc.scale;

            model_mat = glm::translate(model_mat, translation);
            model_mat = glm::scale(model_mat, scale);

            shader->load_matrix("model_mat", model_mat);
        }

        Mesh *mesh = rc.mesh;

        mesh->bind();
        mesh->render();
    }
}

void Renderer2D::end() {
}

void Renderer2D::resize(s32 width, s32 height) {
    glViewport(0, 0, width, height);
    
    glm::mat4 proj_matrix = glm::ortho<f32>(0, width, 0, height, -1, 1);
    Shaders::load_for_all("proj_mat", proj_matrix);
}
