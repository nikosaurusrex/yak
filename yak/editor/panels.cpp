#include "panels.h"

#include "core/engine.h"
#include "editor/project.h"
#include "entity/scene.h"
#include "gfx/framebuffer.h"

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

void SceneView::render(Scene *scene) {
    ImGui::Begin("Scene");

    min_region = ImGui::GetWindowContentRegionMin();
    max_region = ImGui::GetWindowContentRegionMax();
    offset = ImGui::GetWindowPos();

    width = (s32) max_region.x - min_region.x;
    height = (s32) max_region.y - min_region.y;
    framebuffer->bind();

    if (need_resize) {
        framebuffer->resize(width, height);
        Renderer2D::resize(width, height);
        need_resize = false;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    engine->render();

    framebuffer->unbind();

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void *)framebuffer->get(0), 
        ImVec2(pos.x, pos.y), 
        ImVec2(pos.x + width, pos.y + height), 
        ImVec2(0, 1), 
        ImVec2(1, 0)
    );
	ImGui::End();
}

void SceneView::resize() {
    need_resize = true;
}

void SceneView::on_mouse_button(s32 button, s32 action) {
    if (action == GLFW_RELEASE) {
        auto[mx, my] = ImGui::GetMousePos();
        mx -= offset.x + min_region.x;
        my -= offset.y + min_region.y;
        my = height - my;

        if (mx >= 0 && my >= 0 && mx < width && my < height) {
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

    render_component<TransformComponent>(entity, "Transform", [](auto &component) {
        ImGui::DragFloat3("Position", &component.translation.x);
        ImGui::DragFloat3("Scale", &component.scale.x);
    });

    render_component<RendererComponent>(entity, "Renderer", [assets](auto &component) {
        const char *mesh_items[] = { "Quad" };
        static int mesh_item_current = 0;
        if (ImGui::Combo("Mesh", &mesh_item_current, mesh_items, IM_ARRAYSIZE(mesh_items))) {
            component.mesh = Meshes::meshes[mesh_item_current];
        }

        ImGui::ColorEdit4("Color", &component.color.x);

        const char *texture_preview = "Texture";
        static int texture_selected = 0;
        if (component.texture) {
            texture_preview = component.texture->path.c_str();
        }
        if (ImGui::BeginCombo("Texture", texture_preview, 0)) {
            s32 i = 0;
            for (auto kv : assets->textures) {
                bool selected = (texture_selected == i); 
                if (ImGui::Selectable(kv.first.c_str(), selected)) {
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


void RenderStatsPanel::render() {
    ImGui::Begin("Render Statistics");

    ImGui::End();
}
