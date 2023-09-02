#ifndef RENDERER_H
#define RENDERER_H

#include "yakpch.h"

#include "entity/scene.h"

struct Renderer2D {
    static void begin();
    static void render_quad();
    static void end();

    static void resize(s32 width, s32 height);
};

#endif
