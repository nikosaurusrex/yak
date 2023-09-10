#ifndef TEXTURE_H
#define TEXTURE_H

#include "yakpch.h"

struct Texture {
    string path;
    GLuint id;

    Texture(string file_path, GLint format);
    ~Texture();

    void load(string file_path, GLint format);
    void bind(s32 slot);
};

#endif
