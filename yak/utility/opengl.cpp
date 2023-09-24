#include "opengl.h"

#include "common.h"

bool check_opengl_error() {
    bool found = false;
    s32 err = glGetError();
    while (err != GL_NO_ERROR) {
        log_error("glError: %d", err);
        found = true;
        err = glGetError();
    }
    return found;
}

#ifdef _WIN32
void opengl_log(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param) {
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        log_error("[OpenGL Debug HIGH] %s", message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        log_info("[OpenGL Debug MEDIUM] %s", message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        log_info("[OpenGL Debug LOW] %s", message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        log_info("[OpenGL Debug NOTIFICATION] %s", message);
        break;
    }
}

void enable_opengl_debugging() {
    glDebugMessageCallback(opengl_log, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
#else
void enable_opengl_debugging() {}
#endif
