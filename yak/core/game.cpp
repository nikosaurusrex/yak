#include "game.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "entity/components.h"
#include "entity/ecs.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "glm/ext/matrix_transform.hpp"

GameLayer::GameLayer() {
}

void GameLayer::init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameLayer::deinit() {
}

void GameLayer::render(Scene *scene) {
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

            auto translation = tc.translation;
            auto rotation = tc.rotation;
            auto scale = tc.scale;

            glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), translation) * 
                        glm::toMat4(glm::quat(glm::radians(rotation))) *
                        glm::scale(glm::mat4(1.0f), scale);

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
