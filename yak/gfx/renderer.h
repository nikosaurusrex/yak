#ifndef RENDERER_H
#define RENDERER_H

#include "yakpch.h"

#include "core/window.h"
#include "entity/ecs.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

struct Renderer2D {
    Window *window;

    Renderer2D(Window *window);
    ~Renderer2D();

    void init();

    void begin();
    void render_scene(Scene *scene);
    void end();

    void resize(s32 width, s32 height);
};

#endif
