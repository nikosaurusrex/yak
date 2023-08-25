#include "engine.h"

#include "yakpch.h"

#include "core/layer.h"
#include "core/game.h"
#include "core/window.h"
#include "entity/components.h"
#include "entity/ecs.h"
#include "gfx/mesh.h"
#include "gfx/renderer.h"
#include "gfx/shader.h"

Engine::Engine(Window *window) : window(window) {
    scene = new Scene();
    renderer_2d = new Renderer2D(window);
}

Engine::~Engine() {
    Shaders::free();
    Meshes::free();

    for (Layer *layer : layers.layers) {
        delete layer;
    }

    delete window;
    delete scene;
    delete renderer_2d;
}

void Engine::init() {
    window->init();
    window->create();

    Shaders::init();
    Meshes::init();

    renderer_2d->init();

    Entity entity = scene->create_entity("Apple");
    entity.add<MeshComponent>(Meshes::quad);
    entity.add<TransformComponent>(TransformComponent(glm::vec3(50, 50, 1), glm::vec3(100, 100, 1)));
    entity.add<TextureComponent>(new Texture("yak/assets/textures/apple.png", GL_RGBA));

    layers.add(new GameLayer(scene));
}

void Engine::run() {
    running = true;
}

void Engine::loop() {
    while (running) {
        renderer_2d->resize(window->width, window->height);
        update();

        render();

        if (!window->update()) {
            running = false;
        }
    }
    window->destroy();
}

void Engine::update() {
    /* movement */

    /* collision */
}

void Engine::render() {
    renderer_2d->begin();
    for (Layer *layer : layers.layers) {
        layer->render(renderer_2d);
    }
    renderer_2d->end();
}

void Engine::stop() {
    running = false;
}

void Engine::handle_event(Event event) {
    switch (event.type) {
        case Event::RESIZE: {
            window->width = event.width;
            window->height = event.height;
        } break;
    }
}
