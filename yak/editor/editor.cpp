#include "editor.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "core/engine.h"
#include "core/window.h"
#include "entity/components.h"
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
	io.FontDefault = io.Fonts->AddFontFromFileTTF("yak/assets/fonts/Roboto.ttf", 16);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

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

        render();

        if (!window->update()) {
            engine->running = false;
        }
    }

    window->destroy();
}

void Editor::render() {
    renderer->begin();

    ImGuiID dockspace = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    render_menu();

    ImGui::SetNextWindowDockID(dockspace);
    render_scene();
    
    render_scene_hierarchy();

    renderer->end();
}

void Editor::render_menu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Files")) {
            if (ImGui::MenuItem("Exit")) engine->stop();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    } 
}

void Editor::render_scene() {
    ImGui::Begin("Scene");
    s32 width = ImGui::GetContentRegionAvail().x;
    s32 height = ImGui::GetContentRegionAvail().y;

    engine->renderer_2d->resize(width, height);

    framebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    engine->render();
    framebuffer->unbind();

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void *)framebuffer->texture_id, 
        ImVec2(pos.x, pos.y), 
        ImVec2(pos.x + width, pos.y + height), 
        ImVec2(0, 1), 
        ImVec2(1, 0)
    );
	ImGui::End();
}

void Editor::render_scene_hierarchy() {
    ImGui::Begin("Scene Hierarchy");

    auto entities = engine->scene->get_entities();  
    for (auto entity : entities) {
        string tag = entity.get<TagComponent>().tag;

        ImGuiTreeNodeFlags flags = ((selection == entity.id) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)entity.id, flags, tag.data());
		if (ImGui::IsItemClicked()) {
			selection = entity.id;
		}

        bool deleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				deleted = true;

			ImGui::EndPopup();
		}

        if (opened) {
			ImGui::TreePop();
		}

        if (deleted) {
            engine->scene->destroy_entity(entity.id);
            
            if (selection == entity.id)
                selection = 0;
        }
    }

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        selection = 0;

    ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
    if (ImGui::BeginPopupContextWindow(0, popup_flags)) {
        if (ImGui::MenuItem("Create Empty"))
            engine->scene->create_entity("Empty Entity");

        ImGui::EndPopup();
    }

    ImGui::End();
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

