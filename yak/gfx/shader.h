#ifndef SHADER_H
#define SHADER_H

#include "yakpch.h"

#include "math/ymath.h"

struct Shader {
    map<string, GLint> location_cache;
    GLuint id;

    Shader(const char *vert_path, const char *frag_path);
    ~Shader();

    GLuint load(const char *source, GLenum type, const char *type_name);

    void print_shader_log(GLuint shader);
    void print_program_log(GLuint program);

    void use();

    GLint get_uniform_location(string name);
    void load_matrix(string name, mat4 *matrix);
};

struct Shaders {
    static Shader *simple;
    static Shader *texture;

    static void init();
    static void free();

    static void load_for_all(string name, mat4 *mat);
};

#endif