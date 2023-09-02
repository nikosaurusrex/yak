#include "game.h"

#include <glm/gtx/transform.hpp>

#include "entity/components.h"
#include "entity/ecs.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

GameLayer::GameLayer(Scene *scene) : scene(scene) {
}

void GameLayer::init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameLayer::deinit() {
}

void GameLayer::render() {
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
        shader->load_int("in_entity", entity.id);

        if (entity.has<TransformComponent>()) {
            TransformComponent tc = entity.get<TransformComponent>();

            auto model_mat = glm::mat4(1.0);
            auto translation = tc.translation;
            auto scale = tc.scale;

            model_mat = glm::translate(model_mat, translation);
            model_mat = glm::scale(model_mat, scale);

            shader->load_matrix("model_mat", model_mat);
        }

        /* TODO: rework - not have pointer on component
         * and after batch renderer */
        Mesh *mesh = rc.mesh;
        if (mesh == Meshes::quad) {
            Renderer2D::render_quad();
        }
    }
}
