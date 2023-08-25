#include "renderer.h"

#include "entity/components.h"
#include "math/ymath.h"

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
    auto mesh_entities = scene->get_entities_with_component<MeshComponent>();

    for (auto entity : mesh_entities) {
        Shader *shader;

        if (entity.has<TextureComponent>()) {
            shader = Shaders::texture;

            TextureComponent tc = entity.get<TextureComponent>();
            tc.texture->bind(0);
        } else {
            shader = Shaders::simple;
        }

        shader->use();

        if (entity.has<TransformComponent>()) {
            mat4 model_matrix = entity.get<TransformComponent>().transform;

            shader->load_matrix("model_mat", &model_matrix);
        }

        Mesh *mesh = entity.get<MeshComponent>().mesh;

        mesh->bind();
        mesh->render();
    }
}

void Renderer2D::end() {
}

void Renderer2D::resize(s32 width, s32 height) {
    glViewport(0, 0, width, height);
    
    mat4 proj_matrix = mat4::orthographic(0, width, 0, height, -1, 1);
    Shaders::load_for_all("proj_mat", &proj_matrix);
}
