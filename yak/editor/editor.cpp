#include "editor.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "ImGuizmo.h"

#include "core/engine.h"
#include "core/window.h"
#include "editor/camera.h"
#include "editor/project.h"
#include "entity/components.h"
#include "gfx/renderer.h"
#include "gfx/texture.h"

void RendererImGui::init(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    static const ImWchar ranges[] = {
        0x0020, 0x00FF,
        0x2122, 0x2122,
        0x21A9, 0x21A9,
        0x2715, 0x2715,
        0
    };
    
    // io.FontDefault = io.Fonts->AddFontFromFileTTF("yak/assets/fonts/Roboto.ttf", 18);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("yak/assets/fonts/DejaVuSans.ttf", 16, 0, ranges);

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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    IMGUI_CHECKVERSION();
}

void RendererImGui::deinit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void RendererImGui::begin() {
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void RendererImGui::end(s32 width, s32 height) {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((f32)width, (f32)height);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Editor::Editor(Window *window, Project *project) 
    : window(window), project(project) {
    engine = new Engine(window, project->assets);
    scene_view = new SceneView(engine, &selection);
    scene_hierarchy = new SceneHierarchy(&selection);
    properties_panel = new PropertiesPanel(project->assets);
    content_browser = new ContentBrowser(this, project->path);
    render_stats_panel = new RenderStatsPanel();
    toolbar = new Toolbar();
    camera = new Camera(30.0f, 1.0f, 0.1f, 1000.0f);
}

Editor::~Editor() {
    RendererImGui::deinit();

    delete engine;
    delete project;
    delete scene_view;
    delete scene_hierarchy;
    delete properties_panel;
    delete content_browser;
    delete toolbar;
    delete camera;
}

void Editor::init() {
    engine->init();

    project->load();

    RendererImGui::init(window->handle);

    scene_view->init();
    content_browser->init();
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
    RendererImGui::begin();

    ImGuiID dockspace = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    render_menu();

    ImGui::SetNextWindowDockID(dockspace);

    scene_view->render(camera);
    scene_hierarchy->render(engine->scene);
    properties_panel->render(engine->scene, selection);
    content_browser->render();
    render_stats_panel->render();
    toolbar->render(this);

//    DebugConsole::render();

    RendererImGui::end(window->width, window->height);
}

void Editor::render_menu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Files")) {
            if (ImGui::MenuItem("Save", "Cmd+S")) project->save();
            if (ImGui::MenuItem("Exit", "Cmd+Q")) engine->stop();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    } 
}

void Editor::switch_scene(Scene *scene) {
    if (engine->scene) {
        save_scene_file(engine->scene, project->assets);
        delete engine->scene;
    }

    engine->scene = scene;
}

void Editor::handle_event(Event event) {
    if (mode != MODE_EDIT) {
        engine->handle_event(event);
        return;
    }

    switch (event.type) {
        case Event::RESIZE: {
            scene_view->resize();
        } break;
        case Event::MOUSE_BUTTON: {
            scene_view->on_mouse_button(event.button, event.action);
        } break;
        case Event::MOUSE_MOVE: {
        } break;
        case Event::KEY: {
            if (event.action == GLFW_RELEASE) {
                if (event.mods & GLFW_MOD_SUPER) {
                    if (event.button == GLFW_KEY_S) {
                        project->save();
                        if (engine->scene) {
                            save_scene_file(engine->scene, project->assets);
                        }

                    } else if (event.button == GLFW_KEY_Q) {
                        engine->stop();
                    }
                } else {
                    scene_view->on_key_up(event.button);
                }
            }
        } break;
    }
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

