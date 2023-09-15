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
    assets = new Assets("yak/assets/");
}

Engine::Engine(Window *window, Assets *assets)
    : window(window), assets(assets) {
}

Engine::~Engine() {
    Shaders::free();
    Meshes::free();

    for (Layer *layer : layers.layers) {
        delete layer;
    }

    delete window;
    delete assets;
}

void Engine::init() {
    window->init();
    window->create();

    Shaders::init();
    Meshes::init();

    layers.add(new GameLayer());
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
    auto entities = scene->get_entities_with_component<CameraComponent>();
    for (auto &entity : entities) {
        auto &camera_component = entity.get<CameraComponent>();
        if (camera_component.is_main_camera) {
            camera_component.calculate_view_matrix();
            Shaders::load_for_all("view_mat", camera_component.view);
            Shaders::load_for_all("proj_mat", camera_component.projection);
            
            break;
        }
    }

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

void Engine::update_editor() {
    frames++;
    f64 fps_time_now = glfwGetTime();
    f64 fps_time_delta = fps_time_now - fps_time_last;
    if (fps_time_delta >= 1.0f) {
        RenderStats::fps = frames;
        RenderStats::mspf = (fps_time_delta * 1000) / (f32)(frames);

        frames = 0;

        fps_time_last = fps_time_now;
    }
}

void Engine::render() {
    Renderer2D::begin();

    if (scene) {
        for (Layer *layer : layers.layers) {
            layer->render(scene);
        }
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
                
            /* TODO: RECALCULATE MAIN CAMERA */
        } break;
        default:
            break;
    }
}
