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

SceneHierarchy::SceneHierarchy(Entity *selection)
    : selection(selection) {
}

void SceneHierarchy::render(Scene *scene) {
    ImGui::Begin("Scene Hierarchy");

    auto entities = scene->get_entities();  
    for (auto entity : entities) {
        string tag = entity.get<TagComponent>().tag;

        ImGuiTreeNodeFlags flags = ((*selection == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)entity.id, flags, "%s", tag.data());
		if (ImGui::IsItemClicked()) {
			*selection = entity;
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
            scene->destroy_entity(entity.id);
            
            if (*selection == entity)
                *selection = {};
        }
    }

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        *selection = {};

    ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
    if (ImGui::BeginPopupContextWindow(0, popup_flags)) {
        if (ImGui::MenuItem("Create Empty"))
            scene->create_entity("Empty Entity");

        ImGui::EndPopup();
    }

    ImGui::End();
}

void PropertiesPanel::render(Scene *scene, Entity entity) {
    ImGui::Begin("Properties");

    if (!entity) {
        ImGui::End();
        return;
    }

    string &tag = entity.get<TagComponent>().tag;
    static char buf[255];
    strcpy(buf, tag.data());
    if (ImGui::InputText("Tag", buf, sizeof(buf))) {
        tag = string(buf);
    }

    render_components(entity);
    
    ImGui::PushItemWidth(-1);
    if (ImGui::Button("Add Component", ImVec2(-1.0f, 0.0f)))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent")) {

        render_add_component<TransformComponent>(entity, "Transform");
        render_add_component<MeshComponent>(entity, "Mesh");
        render_add_component<TextureComponent>(entity, "Texture");

        ImGui::EndPopup();
    }
	ImGui::PopItemWidth();

    ImGui::End();

}

void PropertiesPanel::render_components(Entity entity) {
    render_component<TransformComponent>(entity, "Transform", [](auto &component) {
        ImGui::DragFloat3("Position", &component.translation.x);
        ImGui::DragFloat3("Scale", &component.scale.x);
    });

    render_component<MeshComponent>(entity, "Mesh", [](auto &component) {
        const char* mesh_items[] = { "Quad" };
        static int mesh_item_current = 0;
        ImGui::Combo("Form", &mesh_item_current, mesh_items, IM_ARRAYSIZE(mesh_items));        
        
        component.mesh = Meshes::meshes[mesh_item_current];
    });

    render_component<TextureComponent>(entity, "Texture", [](auto &component) {
        
    });
}

template <typename T, typename Callback>
void PropertiesPanel::render_component(Entity entity, string name, Callback callback) {
    ImGuiTreeNodeFlags tn_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if (entity.has<T>()) {
        auto &component = entity.get<T>();

        ImVec2 region = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), tn_flags, "%s", name.data());
        ImGui::PopStyleVar();

        f32 remove_button_size = 22.0f + 4.0f * 2.0f;
        ImGui::SameLine(region.x - remove_button_size * 0.5f);
        bool remove = false;
        if (ImGui::Button("-", ImVec2{ remove_button_size, remove_button_size })) {
            remove = true;
        }

        if (open) {
            callback(component);

            ImGui::TreePop();
        }

        if (remove)
            entity.remove<T>();
    }
}

template <typename T>
void PropertiesPanel::render_add_component(Entity entity, string name) {
    if (!entity.has<T>()) {
        if (ImGui::MenuItem(name.data())) {
            entity.add<T>(T());
            ImGui::CloseCurrentPopup();
        }
    } 
}

void ContentBrowser::render() {
    ImGui::Begin("Content Browser");

    ImGui::End();
}

Editor::Editor(Window *window) : window(window) {
    engine = new Engine(window);
    renderer = new RendererImGui(window);
    scene_hierarchy = new SceneHierarchy(&selection);
    properties_panel = new PropertiesPanel();
    content_browser = new ContentBrowser();
}
Editor::~Editor() {
    delete engine;
    delete renderer;
    delete scene_hierarchy;
    delete properties_panel;
    delete content_browser;
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

    scene_hierarchy->render(engine->scene);
    properties_panel->render(engine->scene, selection);
    content_browser->render();

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

void Editor::handle_event(Event event) {
    switch (event.type) {
        case Event::RESIZE: {
            framebuffer->resize(event.width, event.height);
        } break;
    }

    engine->handle_event(event);
}

void run_editor() {
    Window *window = new Window("Engine", 1920, 1080);
    Editor editor(window);

    editor.init();
    window->set_event_handler(&editor);
    // window->expand();
    editor.run();
}

