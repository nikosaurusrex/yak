#ifndef RENDERER_H
#define RENDERER_H

#include "yakpch.h"

#include "entity/scene.h"

struct RenderStats {
    static u64 fps;
    static f32 mspf;
    static u64 render_calls;
    static u64 vertices;
    static u64 indices;
};

struct Renderer2D {
    static void begin();
    static void render_quad();
    static void end();

    static void resize(s32 width, s32 height);
};

#endif
