#include "editor.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "core/engine.h"
#include "core/window.h"
#include "editor/project.h"
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
    
	io.FontDefault = io.Fonts->AddFontFromFileTTF("yak/assets/fonts/Roboto.ttf", 18);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
    colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Border
    colors[ImGuiCol_Border] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};

    // Text
    colors[ImGuiCol_Text] = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
    colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_CheckMark] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};

    // Popups
    colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
    colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.13f, 0.13, 0.17, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
    colors[ImGuiCol_ScrollbarGrab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};

    // Seperator
    colors[ImGuiCol_Separator] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};
    colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
    colors[ImGuiCol_SeparatorActive] = ImVec4{0.84f, 0.58f, 1.0f, 1.0f};

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
    colors[ImGuiCol_ResizeGripActive] = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};

    // Docking
    colors[ImGuiCol_DockingPreview] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};

    auto &style = ImGui::GetStyle();
    style.TabRounding = 4;
    style.ScrollbarRounding = 9;
    style.WindowRounding = 7;
    style.GrabRounding = 3;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ChildRounding = 4;

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

Editor::Editor(Window *window, Project *project) 
    : window(window), project(project) {
    engine = new Engine(window, project->scene, project->assets);
    renderer = new RendererImGui(window);
    scene_view = new SceneView(engine, &selection);
    scene_hierarchy = new SceneHierarchy(&selection);
    content_browser = new ContentBrowser();
    properties_panel = new PropertiesPanel(project->assets);
}

Editor::~Editor() {
    delete engine;
    delete renderer;
    delete project;
    delete scene_view;
    delete scene_hierarchy;
    delete properties_panel;
    delete content_browser;
}

void Editor::init() {
    engine->init();

    project->load();

    renderer->init();

    scene_view->init();
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

    scene_view->render(engine->scene);
    scene_hierarchy->render(engine->scene);
    properties_panel->render(engine->scene, selection);
    content_browser->render();

    renderer->end();
}

void Editor::render_menu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Files")) {
            if (ImGui::MenuItem("Save")) project->save();
            if (ImGui::MenuItem("Exit")) engine->stop();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    } 
}

void Editor::handle_event(Event event) {
    switch (event.type) {
        case Event::RESIZE: {
            scene_view->resize();
        } break;
        case Event::MOUSE_BUTTON: {
            scene_view->on_mouse_button(event.button, event.action);
        } break;
        case Event::MOUSE_MOVE: {
        } break;
    }

    engine->handle_event(event);
}

void run_editor() {
    Window *window = new Window("Engine", 1920, 1080);
    Project *project = new Project("/Users/niko/Desktop/example/");
    Editor editor(window, project);

    editor.init();
    window->set_event_handler(&editor);
    // window->expand();
    editor.run();
}

