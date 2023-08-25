#include "window.h"

#include "core/event.h"

Window::Window(const char *title, s32 width, s32 height)
    : title(title), width(width), height(height) {}

void Window::init() {
    if (!glfwInit()) {
        log_fatal("Failed to initialize GLFW!");
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}

void Window::create() {
    handle = glfwCreateWindow(width, height, title, 0, 0);
    if (!handle) {
        log_fatal("Failed to create GLFW window!");
    }

    const GLFWvidmode *vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(handle, vid_mode->width / 2 - width / 2, vid_mode->height / 2 - height / 2);
    glfwMakeContextCurrent(handle);

    log_info("OpenGL Info:");
    log_info("  Vendor: %s", glGetString(GL_VENDOR));
    log_info("  Renderer: %s", glGetString(GL_RENDERER));
    log_info("  Version: %s", glGetString(GL_VERSION));

#ifdef _WIN32
    if (glewInit() != GLEW_OK) {
        log_fatal("Failed to initialize GLEW!");
    }
#endif

    glfwSetWindowUserPointer(handle, 0);

    glfwSetWindowSizeCallback(handle, window_resize);

    glfwShowWindow(handle);
}

bool Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(handle);

    return !glfwWindowShouldClose(handle);
}

void Window::destroy() {
    glfwDestroyWindow(handle);
    glfwTerminate();
}

void Window::set_event_handler(EventHandler *handler) {
    glfwSetWindowUserPointer(handle, handler);
}

void window_resize(GLFWwindow* handle, s32 width, s32 height) {
    EventHandler *handler = (EventHandler *) glfwGetWindowUserPointer(handle);

    if (!handler) return;

    Event event;
    event.type = Event::RESIZE;
    event.width = width;
    event.height = height;

    handler->handle_event(event);
}