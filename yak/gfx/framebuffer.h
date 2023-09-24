#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "common.h"

struct Framebuffer {
    s32 width;
    s32 height;
    GLuint id = 0;
    array<GLint> color_attachments_types;
    array<GLuint> color_attachments;
    GLint depth_attachment_type = 0;
    GLuint depth_attachment;

    Framebuffer(s32 width, s32 height, array<GLint> attachments);
    ~Framebuffer();
    
    void reload();

    void bind();
    void unbind();

    void resize(s32 width, s32 height);

    GLuint get(u32 index);
    s32 read(u32 index, s32 x, s32 y);
    void clear(u32 index, s32 value);
};

#endif
