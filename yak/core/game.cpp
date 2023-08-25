#include "game.h"

GameLayer::GameLayer(Scene *scene) : scene(scene) {
}

void GameLayer::init() {
}

void GameLayer::deinit() {
}

void GameLayer::render(Renderer2D *renderer) {
    renderer->render_scene(scene);
}