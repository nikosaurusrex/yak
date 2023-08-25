#ifndef GAME_H
#define GAME_H

#include "layer.h"

struct GameLayer : public Layer {
    Scene *scene;

    GameLayer(Scene *scene);

    virtual void init() override;
    virtual void deinit() override;

    virtual void render(Renderer2D *renderer) override;
};

#endif
