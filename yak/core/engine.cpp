#include "engine.h"

#include "common.h"

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

	log_info("Create Engine");
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
	log_info("Init Engine");

    window->init();
    window->create();

    Shaders::init();
    Meshes::init();

    layers.add(new GameLayer());
}

void Engine::run() {
	log_info("Run Engine");

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
    auto camera_entities = scene->get_entities_with_component<CameraComponent>();
    for (auto &entity : camera_entities) {
        auto &camera_component = entity.get<CameraComponent>();
        if (camera_component.is_main_camera) {
            camera_component.calculate_view_matrix();
            Shaders::load_for_all("view_mat", camera_component.view);
            Shaders::load_for_all("proj_mat", camera_component.projection);
            
            break;
        }
    }

    auto script_entities = scene->get_entities_with_component<ScriptComponent>();
    for (auto &entity : script_entities) {
        if (!entity.has<TransformComponent>()) {
            continue;
        }

        auto &script_component = entity.get<ScriptComponent>();
        auto &transform_component = entity.get<TransformComponent>();

        auto script = script_component.script;

		if (!script->loaded) {
			continue;
		}

        string tag = entity.get<TagComponent>().tag;
        glm::vec3 *camera_pos = 0;
        if (entity.has<CameraComponent>()) {
            camera_pos = &entity.get<CameraComponent>().position;
        }

        if (script->update_function) {
            script->set_function(tag.c_str(), &transform_component, camera_pos);
            script->update_function();
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
	log_info("Log Engine");
	
    running = false;
}

void Engine::handle_event(Event event) {
    switch (event.type) {
        case Event::RESIZE: {
            window->width = event.width;
            window->height = event.height;
                
            /* TODO: RECALCULATE MAIN CAMERA */
        } break;
        case Event::KEY: {
            if (event.action == GLFW_PRESS) {
                auto script_entities = scene->get_entities_with_component<ScriptComponent>();
                for (auto &entity : script_entities) {
                    auto &script_component = entity.get<ScriptComponent>();
                    auto script = script_component.script;

                    if (script->on_key_down_function) {
                        script->on_key_down_function(event.button);
                    }
                }
            }
            if (event.action == GLFW_RELEASE) {
                auto script_entities = scene->get_entities_with_component<ScriptComponent>();
                for (auto &entity : script_entities) {
                    auto &script_component = entity.get<ScriptComponent>();
                    auto script = script_component.script;

                    if (script->on_key_release_function) {
                        script->on_key_release_function(event.button);
                    }
                }
            }
        } break;
        default:
            break;
    }
}
