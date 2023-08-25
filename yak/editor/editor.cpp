#include "editor.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "core/engine.h"
#include "core/window.h"
#include "gfx/renderer.h"
#include "gfx/texture.h"

RendererImGui::RendererImGui(Window *window) : window(window) {

}

RendererImGui::~RendererImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void RendererImGui::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window->handle, true);
    ImGui_ImplOpenGL3_Init("#version 410");IMGUI_CHECKVERSION();
}

void RendererImGui::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClear(GL_COLOR_BUFFER_BIT);
}

void RendererImGui::end() {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)window->width, (float)window->height);

    // Rendering
    ImGui::Render();
    glViewport(0, 0, window->width, window->height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Editor::Editor(Window *window) : window(window) {
    engine = new Engine(window);
    renderer = new RendererImGui(window);
}
Editor::~Editor() {
    delete engine;
    delete renderer;
    delete framebuffer;
}

void Editor::init() {
    engine->init();

    renderer->init();

    framebuffer = new Framebuffer(window->width, window->height);
}

void Editor::run() {
    engine->run();

    while (engine->running) {
        engine->update();

        framebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        engine->render();
        framebuffer->unbind();

        render();

        if (!window->update()) {
            engine->running = false;
        }
    }

    window->destroy();
}

void Editor::render() {
    renderer->begin();

    ImGui::Begin("Dockspace");

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Files")) {
            if (ImGui::MenuItem("Exit")) engine->stop();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    } 

    ImGui::Begin("Scene");
    s32 width = ImGui::GetContentRegionAvail().x;
    s32 height = ImGui::GetContentRegionAvail().y;

    engine->renderer_2d->resize(width, height);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void *)framebuffer->texture_id, 
        ImVec2(pos.x, pos.y), 
        ImVec2(pos.x + width, pos.y + height), 
        ImVec2(0, 1), 
        ImVec2(1, 0)
    );
	ImGui::End();

    ImGui::End();

    renderer->end();
}

void Editor::handle_event(Event event) {
    switch (event.type) {
        case Event::RESIZE: {
            framebuffer->resize(event.width, event.height);
        } break;
    }

    engine->handle_event(event);
}

void run_editor() {
    Window *window = new Window("Engine", 800, 600);
    Editor editor(window);

    editor.init();
    window->set_event_handler(&editor);
    editor.run();
}

