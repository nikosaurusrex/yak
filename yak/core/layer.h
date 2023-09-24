#ifndef YAK_LAYER_H
#define YAK_LAYER_H

#include "common.h"

#include "entity/scene.h"
#include "gfx/renderer.h"

struct Layer {

    Layer() = default;
    virtual ~Layer() = default;

    virtual void init() = 0;
    virtual void deinit() = 0;

    virtual void render(Scene *scene) = 0;
};

struct Layers {
    array<Layer *> layers;

    void add(Layer *layer) {
        layer->init();
        layers.push_back(layer);
    }

    void remove(Layer *layer) {
        auto it = std::find(layers.begin(), layers.end(), layer);
        layers.erase(it);
        layer->deinit();
    }
};

#endif
