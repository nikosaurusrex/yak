#ifndef TEXTURE_H
#define TEXTURE_H

#include "yakpch.h"

struct Texture {
    string path;
    GLuint id;

    Texture(string file_path, GLint format);
    Texture(string full_path, string relative_path, GLint format);
    ~Texture();

    void load(string file_path, GLint format);
    void bind(s32 slot);
};

struct Framebuffer {
    GLuint fbo;
    GLuint rbo;
    GLuint texture_id;

    Framebuffer(s32 width, s32 height);
    ~Framebuffer();

    void bind();
    void unbind();

    void resize(s32 width, s32 height);
};

#endif
