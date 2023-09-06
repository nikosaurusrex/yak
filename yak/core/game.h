#ifndef GAME_H
#define GAME_H

#include "layer.h"

struct GameLayer : public Layer {
    GameLayer();

    virtual void init() override;
    virtual void deinit() override;

    virtual void render(Scene *scene) override;
};

#endif
