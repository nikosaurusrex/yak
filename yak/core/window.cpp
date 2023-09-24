#include "window.h"

#include "core/event.h"

static void window_resize(GLFWwindow *handle, s32 width, s32 height) {
    EventHandler *handler = (EventHandler *) glfwGetWindowUserPointer(handle);

    if (!handler) return;

    Event event;
    event.type = Event::RESIZE;
    event.width = width;
    event.height = height;

    handler->handle_event(event);
}

static void mouse_button_callback(GLFWwindow *handle, s32 button, s32 action, s32 mods) {
    EventHandler *handler = (EventHandler *) glfwGetWindowUserPointer(handle);

    if (!handler) return;

    Event event;
    event.type = Event::MOUSE_BUTTON;
    event.button = button;
    event.action = action;
    event.mods = mods;

    handler->handle_event(event);
}

static void cursor_position_callback(GLFWwindow *handle, f64 xpos, f64 ypos) {
    EventHandler *handler = (EventHandler *) glfwGetWindowUserPointer(handle);

    if (!handler) return;

    Event event;
    event.type = Event::MOUSE_MOVE;
    event.xpos = xpos;
    event.ypos = ypos;

    handler->handle_event(event);
}

static void key_callback(GLFWwindow *handle, s32 key, s32 scancode, s32 action, s32 mods) {
    EventHandler *handler = (EventHandler *) glfwGetWindowUserPointer(handle);

    if (!handler) return;

    Event event;
    event.type = Event::KEY;
    event.button = key;
    event.action = action;
    event.mods = mods;

    handler->handle_event(event);
}

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

    glewInit();

    log_info("OpenGL Info:");
    log_info("  Vendor: %s", glGetString(GL_VENDOR));
    log_info("  Renderer: %s", glGetString(GL_RENDERER));
    log_info("  Version: %s", glGetString(GL_VERSION));

    glfwSetWindowUserPointer(handle, 0);

    glfwSetWindowSizeCallback(handle, window_resize);
    glfwSetMouseButtonCallback(handle, mouse_button_callback);
    glfwSetCursorPosCallback(handle, cursor_position_callback);
    glfwSetKeyCallback(handle, key_callback);

    glfwShowWindow(handle);
}

bool Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(handle);

    return !glfwWindowShouldClose(handle);
}

void Window::expand() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
 
    glfwSetWindowMonitor(handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void Window::destroy() {
    glfwDestroyWindow(handle);
    glfwTerminate();
}

void Window::set_event_handler(EventHandler *handler) {
    glfwSetWindowUserPointer(handle, handler);
}
