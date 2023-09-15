#include "panels.h"

#include "glm/gtc/type_ptr.hpp"

#include "ImGuizmo.h"

#include "core/engine.h"
#include "editor/camera.h"
#include "editor/editor.h"
#include "editor/project.h"
#include "entity/scene.h"
#include "gfx/framebuffer.h"
#include "gfx/shader.h"

SceneView::SceneView(Engine *engine, Entity *selection)
    : engine(engine), selection(selection) {
}

SceneView::~SceneView() {
    delete framebuffer;
}

void SceneView::init() {
    framebuffer = new Framebuffer(
        800,
        600,
        {GL_RGBA, GL_RED_INTEGER, GL_DEPTH24_STENCIL8}
    );
}

void SceneView::render(Camera *camera) {
    ImGui::Begin("Scene");

    offset = ImGui::GetCursorScreenPos();
    region = ImGui::GetContentRegionAvail();

    if (region.x != last_region.x || region.y != last_region.y) {
        need_resize = true;
    }

    last_region = region;

    s32 width = (s32) region.x;
    s32 height = (s32) region.y;
    framebuffer->bind();

    if (need_resize) {
        framebuffer->resize(width, height);
        Renderer2D::resize(width, height);
        camera->resize(width, height);

        proj_mat = camera->projection;
        view_mat = camera->view;

        Shaders::load_for_all("view_mat", view_mat);
        Shaders::load_for_all("proj_mat", proj_mat);

        need_resize = false;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    engine->render();

    framebuffer->unbind();

    ImGui::GetWindowDrawList()->AddImage(
        (void *) framebuffer->get(0), 
        ImVec2(offset.x, offset.y), 
        ImVec2(offset.x + width, offset.y + height), 
        ImVec2(0, 1), 
        ImVec2(1, 0)
    );

    if (*selection) {
        Entity entity = *selection;
        if (entity.has<TransformComponent>()) {
            TransformComponent &tc = entity.get<TransformComponent>();
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
	    ImGuizmo::Enable(true);

            // ImGuizmo::SetRect(offset.x, offset.y, width, height);
            f32 w = ImGui::GetWindowWidth();
            f32 h = ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, w, h);

            glm::mat4 translate = tc.transform();

            ImGuizmo::Manipulate(
                glm::value_ptr(view_mat),
                glm::value_ptr(proj_mat),
                (ImGuizmo::OPERATION) guizmo_operation,
                ImGuizmo::LOCAL,
                glm::value_ptr(translate)
            );

            if (ImGuizmo::IsUsing()) {
                ImGuizmo::DecomposeMatrixToComponents(
                    glm::value_ptr(translate),
                    glm::value_ptr(tc.translation),
                    glm::value_ptr(tc.rotation),
                    glm::value_ptr(tc.scale)
                );
            }
        }
    }
    
    ImGui::End();
}

void SceneView::resize() {
    need_resize = true;
}

void SceneView::on_mouse_button(s32 button, s32 action) {
    if (!engine->scene) return;

    if (action == GLFW_RELEASE) {
        auto[mx, my] = ImGui::GetMousePos();
        mx -= offset.x;
        my -= offset.y;
        my = region.y - my;

        if (mx >= 0 && my >= 0 && mx < region.x && my < region.y) {
            framebuffer->bind();
            s32 pixel = framebuffer->read(1, mx, my);
            framebuffer->unbind();

            if (pixel != 0) {
                *selection = Entity(&engine->scene->registry, pixel);
            } else {
                *selection = Entity();
            }
        }
    }
}

void SceneView::on_key_up(s32 key) {
    if (key == GLFW_KEY_T) {
        guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
    } else if (key == GLFW_KEY_R) {
        guizmo_operation = ImGuizmo::OPERATION::ROTATE;
    } else if (key == GLFW_KEY_S) {
        guizmo_operation = ImGuizmo::OPERATION::SCALE;
    }
}

SceneHierarchy::SceneHierarchy(Entity *selection)
    : selection(selection) {
}

void SceneHierarchy::render(Scene *scene) {
    ImGui::Begin("Scene Hierarchy");
    if (!scene) {
        ImGui::End();
        return;
    }

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

PropertiesPanel::PropertiesPanel(Assets *assets) : assets(assets) {
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
        render_add_component<RendererComponent>(entity, "Renderer");

        ImGui::EndPopup();
    }
    
    ImGui::PopItemWidth();

    ImGui::End();

}

void PropertiesPanel::render_components(Entity entity) {
    Assets *assets = this->assets;

    ImGui::PushID(entity.id);

    render_component<TransformComponent>(entity, "Transform", [this](auto &component) {
        render_vec_xyz(&component.translation, "Position", "##position");
        render_vec_xyz(&component.rotation, "Rotation", "##rotation");
        render_vec_xyz(&component.scale, "Scale", "##scale");
    });

    render_component<RendererComponent>(entity, "Renderer", [this, assets](auto &component) {
        bool remove = render_property("Mesh", false, [&component]{
            const char *mesh_items[] = { "Quad" };
            static int mesh_item_current = 0;
            
            if (ImGui::Combo("##mesh", &mesh_item_current, mesh_items, IM_ARRAYSIZE(mesh_items))) {
                component.mesh = Meshes::meshes[mesh_item_current];
            }
        });

        remove = render_property("Color", false, [&component] {
            ImGui::ColorEdit4("##color", &component.color.x);
        });

        remove = render_property("Texture", true, [&component, assets] {
            const char *texture_preview = "<None>";
            static int texture_selected = 0;
            if (component.texture) {
                auto tex_name = fs::path(component.texture->path).filename();
                texture_preview = tex_name.c_str();
            }
            if (ImGui::BeginCombo("##texture", texture_preview, 0)) {
                s32 i = 0;
                for (auto kv : assets->textures) {
                    bool selected = (texture_selected == i); 
                    auto tex_name = fs::path(kv.first).filename();
                    if (ImGui::Selectable(tex_name.c_str(), selected)) {
                        texture_selected = i;
                        component.texture = kv.second;
                    }

                    if (selected)
                        ImGui::SetItemDefaultFocus();

                    i++;
                }

                ImGui::EndCombo();
            }
        });
        if (remove) {
            component.texture = 0;
        }
    });

    ImGui::PopID();
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

        ImGuiIO& io = ImGui::GetIO();
        f32 font_size = io.FontDefault->FontSize;
        f32 remove_button_size = font_size + 4.0f * 2.0f;
        ImGui::SameLine(region.x - remove_button_size * 0.5f);
        bool remove = false;
        if (ImGui::Button("\u2715", ImVec2{ remove_button_size, remove_button_size })) {
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

template <typename Callback>
bool PropertiesPanel::render_property(const char *name, bool has_remove_button, Callback callback) {
    ImGui::PushID(name);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
    ImGui::Text("%s", name);
    ImGui::NextColumn();

    ImVec2 region = ImGui::GetContentRegionAvail();
    f32 width = region.x;
    f32 remove_button_size;

    if (has_remove_button) {
        ImGuiIO& io = ImGui::GetIO();
        f32 font_size = io.FontDefault->FontSize;
        remove_button_size = font_size + 4.0f * 2.0f;
        width -= remove_button_size;
    }

    ImGui::PushItemWidth(width);

    callback();    

    ImGui::PopItemWidth();

    bool remove = false;
    if (has_remove_button) {
        ImGui::SameLine(0.0f, 0.0f);

        if (ImGui::Button("\u2715", ImVec2{ remove_button_size, remove_button_size })) {
            remove = true;
        }
    }

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();

    return remove;
}

void PropertiesPanel::render_vec_xyz(glm::vec3 *vec, const char *label, const char *id) {
    ImVec2 region = ImGui::GetContentRegionAvail();
    ImGui::PushID(id);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::Text("%s", label);
    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    f32 label_width = ImGui::CalcTextSize("M").x;
    ImGui::PushItemWidth(((region.x - 100.0f) / 3) - label_width);

    ImGui::PushStyleColor(ImGuiCol_Button, {1.0f, 0.75f, 0.81f, 0.2f});
    ImGui::Button("X"); ImGui::SameLine();
    ImGui::DragFloat("##x", &vec->x, 0.05f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();

    ImGui::Button("Y"); ImGui::SameLine();
    ImGui::DragFloat("##y", &vec->y, 0.05f, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();

    ImGui::Button("Z"); ImGui::SameLine();
    ImGui::DragFloat("##z", &vec->z, 0.05f, 0.0f, 0.0f, "%.2f");
    ImGui::PopStyleColor();

    ImGui::PopItemWidth();
    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
}

void Toolbar::render(Editor *editor) {
    ImGui::Begin("toolbar", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);


    if (editor->mode == MODE_EDIT) {
        if (ImGui::Button("Play")) {
            editor->mode = MODE_PLAY;
            editor->selection = {};
        }
    } else {
        if (ImGui::Button("Edit")) {
            editor->mode = MODE_EDIT;
        }
    }

    ImGui::End();
}


ContentBrowser::ContentBrowser(Editor *editor, string project_path)
    : editor(editor), project_path(project_path), path(project_path) {
}

ContentBrowser::~ContentBrowser() {
    delete img_file;
    delete img_folder;
}

void ContentBrowser::init() {
    img_file = new Texture("yak/assets/textures/file.png", GL_RGBA);
    img_folder = new Texture("yak/assets/textures/folder.png", GL_RGBA);
}

void ContentBrowser::render() {
    ImGui::Begin("Content Browser");

    f32 font_size = ImGui::GetFontSize();
    if (ImGui::Button("\u21A9")) {
        if (!fs::equivalent(project_path, path)) {
            path = path.parent_path();
        }
    }

    ImGui::SameLine();
    auto relative_path = fs::relative(path, project_path);
    ImGui::Text("%s", relative_path.c_str());

    f32 icon_size = 80.0f;
    f32 margin = 10.0f;
    f32 cell_size = icon_size + margin;
    f32 cells_x = (s32)(ImGui::GetContentRegionAvail().x / cell_size);
    if (cells_x < 1) {
        cells_x = 1;
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

    ImGui::Columns(cells_x, 0, false);

    bool open_text_input = false;
    static string text_input_mode = "";
    static string text_input_context = "";

    set<fs::path> directories; 
    set<fs::path> files;

    for (auto &dir : fs::directory_iterator(path)) {
        if (dir.is_directory()) {
            directories.insert(dir.path());
        } else {
            files.insert(dir.path());
        }
    }

    for (auto &dir : directories) {
        auto file_name = dir.filename();
        auto name = file_name.string();

        ImGui::PushID(name.c_str());

        Texture *img = img_folder;
        ImGui::ImageButton((ImTextureID) img->id, { icon_size, icon_size }, { 0, 1 }, { 1, 0 });
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            path /= file_name;
        }

        item_popup(text_input_mode, text_input_context, open_text_input, file_name);

        ImGui::TextWrapped("%s", name.c_str());
        ImGui::PopID();

        ImGui::NextColumn();
    }

    for (auto &file : files) {
        auto file_name = file.filename();
        auto name = file_name.string();

        if (name == ".DS_Store") {
            continue;
        }

        auto file_path = path / file_name;
        auto ext = file_path.extension();
        auto assets = editor->project->assets;
        Texture *img = img_file;

        ImGui::PushID(name.c_str());

        if (ext == ".png") {
            Texture *tex = assets->get(file_path);
            if (tex) {
                img = tex;
            }
        }

        ImGui::ImageButton((ImTextureID) img->id, { icon_size, icon_size }, { 0, 1 }, { 1, 0 });
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (ext == ".scene") {
                Scene *scene = load_scene_file(file_path.string(), editor->project->assets);
                editor->switch_scene(scene);
            } else if (ext == ".png") {
                assets->load_texture(file_path);
            }
        }

        item_popup(text_input_mode, text_input_context, open_text_input, file_name);

        ImGui::TextWrapped("%s", name.c_str());
        ImGui::PopID();

        ImGui::NextColumn();
    }

    ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
    if (ImGui::BeginPopupContextWindow(0, popup_flags)) {
        if (ImGui::MenuItem("Create Folder")) {
            open_text_input = true;
            text_input_mode = "create_folder";
        }
        ImGui::EndPopup();
    }

    if (open_text_input) {
        ImGui::OpenPopup("POPUP_TEXT_INPUT");
    }

    if (ImGui::BeginPopup("POPUP_TEXT_INPUT")) {
        ImGui::Text("Name");
        static char buf[255];
        ImGui::InputText("##text_input", buf, sizeof(buf));
        if (ImGui::Button("Confirm")) {
            ImGui::CloseCurrentPopup();

            if (text_input_mode == "create_folder") {
                auto new_dir = path / buf;
                fs::create_directory(new_dir);
            } else if (text_input_mode == "rename") {
                auto new_dir = path / buf;
                fs::rename(text_input_context, new_dir); 
            }

            buf[0] = 0;
        }
        ImGui::EndPopup();
    }

    ImGui::PopStyleColor();
    ImGui::Columns(1);

    ImGui::End();
}

void ContentBrowser::item_popup(string &text_input_mode, string &text_input_context, bool &open_text_input, fs::path &file_name) {
    if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Rename")) {
            auto file_path = path / file_name;

            open_text_input = true;
            text_input_mode = "rename";
            text_input_context = file_path;
        }

        if (ImGui::MenuItem("Delete")) {
            auto file_path = path / file_name;
            fs::remove_all(file_path);
        }
        ImGui::EndPopup();
    }
}

void RenderStatsPanel::render() {
    ImGui::Begin("Render Statistics");

    ImGui::Text("FPS %llu", RenderStats::fps);
    ImGui::Text("ms/frame %f", RenderStats::mspf);
    ImGui::Text("Render Calls %llu", RenderStats::render_calls);
    ImGui::Text("Vertices %llu", RenderStats::vertices);
    ImGui::Text("Indices %llu", RenderStats::indices);

    ImGui::End();
}

void DebugConsole::render() {
    ImGui::Begin("Debug Console");

    ImGui::End();
}
