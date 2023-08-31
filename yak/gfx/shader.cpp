#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "utility/io.h"
#include "utility/opengl.h"

Shader::Shader(const char *vert_path, const char *frag_path) {
    char *vert_source = read_entire_file(vert_path);
    char *frag_source = read_entire_file(frag_path);

    if (!vert_source) {
        log_fatal("Failed to read vertex shader file '%s'", vert_path);
    }

    if (!frag_source) {
        log_fatal("Failed to read fragment shader file '%s'", frag_path);
    }

    GLuint vert_shader = load(vert_source, GL_VERTEX_SHADER, "Vertex Shader");
    GLuint frag_shader = load(frag_source, GL_FRAGMENT_SHADER, "Fragment Shader");

    id = glCreateProgram();

    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);

    glLinkProgram(id);
    check_opengl_error();

    GLint linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        log_error("Linking error");
        print_program_log(id);
    }

	glDetachShader(id, vert_shader);
	glDetachShader(id, frag_shader);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    delete[] vert_source;
    delete[] frag_source;
}

Shader::~Shader() {
    glDeleteProgram(id);
}

GLuint Shader::load(const char *source, GLenum type, const char *type_name) {
    GLint compiled;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);

    glCompileShader(shader);
    check_opengl_error();
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != 1) {
        print_shader_log(shader);
        log_fatal("%s Compilation failed", type_name);
    }

    return shader;
}

void Shader::print_shader_log(GLuint shader) {
    s32 length = 0, chars_written = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return;
    }

    log = new char[length];
    glGetShaderInfoLog(shader, length, &chars_written, log);
    log_error("Shader Info Log: %s", log);
    delete[] log;
}

void Shader::print_program_log(GLuint prog) {
    s32 length = 0, chars_written = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return;
    }

    log = new char[length];
    glGetProgramInfoLog(prog, length, &chars_written, log);
    log_error("Program Info Log: %s", log);
    delete[] log;
}

void Shader::use() {
    glUseProgram(id);
}

GLint Shader::get_uniform_location(string name) {
    auto it = location_cache.find(name);
    if (it != location_cache.end()) {
        return it->second;
    }

    GLint location = glGetUniformLocation(id, name.data());
    location_cache.insert({name, location});
    return location;
}

void Shader::load_int(string name, s32 value) {
    GLint location = get_uniform_location(name);
    glUniform1i(location, value);
}

void Shader::load_vec4(string name, glm::vec4 vec) {
    GLint location = get_uniform_location(name);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::load_matrix(string name, glm::mat4 matrix) {
    GLint location = get_uniform_location(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader *Shaders::simple = 0;
Shader *Shaders::texture = 0;

void Shaders::init() {
    Shaders::simple = new Shader("yak/assets/shaders/simple.vert", "yak/assets/shaders/simple.frag");
    Shaders::texture = new Shader("yak/assets/shaders/texture.vert", "yak/assets/shaders/texture.frag");
}

void Shaders::free() {
    delete Shaders::simple;
}

void Shaders::load_for_all(string name, glm::mat4 mat) {
    Shaders::simple->use();
    Shaders::simple->load_matrix(name, mat);

    Shaders::texture->use();
    Shaders::texture->load_matrix(name, mat);
}
