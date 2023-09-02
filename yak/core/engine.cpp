#include "engine.h"

#include "yakpch.h"

#include "core/layer.h"
#include "core/game.h"
#include "core/window.h"
#include "entity/components.h"
#include "gfx/mesh.h"
#include "gfx/renderer.h"
#include "gfx/shader.h"

Engine::Engine(Window *window) : window(window) {
    scene = new Scene();
    assets = new Assets("yak/assets/");
}

Engine::Engine(Window *window, Scene *scene, Assets *assets)
    : window(window), scene(scene), assets(assets) {
}

Engine::~Engine() {
    Shaders::free();
    Meshes::free();

    for (Layer *layer : layers.layers) {
        delete layer;
    }

    delete window;
    delete scene;
    delete assets;
}

void Engine::init() {
    window->init();
    window->create();

    Shaders::init();
    Meshes::init();

    layers.add(new GameLayer(scene));
}

void Engine::run() {
    running = true;
}

void Engine::loop() {
    while (running) {
        Renderer2D::resize(window->width, window->height);
        update();

        render();

        if (!window->update()) {
            running = false;
        }
    }
    window->destroy();
}

void Engine::update() {
    frames++;
    f64 fps_time_now = glfwGetTime();
    f64 fps_time_delta = fps_time_now - fps_time_last;
    if (fps_time_delta >= 1.0f) {
        RenderStats::fps = frames;
        RenderStats::mspf = (fps_time_delta * 1000) / (f32)(frames);

        frames = 0;

        fps_time_last = fps_time_now;
    }

    /* movement */

    /* collision */
}

void Engine::render() {
    Renderer2D::begin();
    for (Layer *layer : layers.layers) {
        layer->render();
    }
    Renderer2D::end();
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
