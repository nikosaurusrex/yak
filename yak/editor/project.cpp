#include "project.h"

#include <filesystem>
#include <sstream>

#include "entity/components.h"

SceneFile::SceneFile(string path, Scene *scene, Assets *assets)
    : path(path), scene(scene), assets(assets) {
}

void SceneFile::read() {
    std::ifstream file(path);

    string line;

    while (std::getline(file, line)) {
        if (line == "[Entities]") {
            std::getline(file, line);

            while (line != "[EntitiesEnd]") {
                if (line == "[Entity]") {
                    std::getline(file, line);
                    Entity entity = scene->create_entity(line);
                    
                    std::getline(file, line);
                    while (line != "[EntityEnd]") {
                        if (line == "[Transform]") {
                            glm::vec3 translation = read_vec3(file);
                            glm::vec3 rotation = read_vec3(file);
                            glm::vec3 scale = read_vec3(file);

                            entity.add<TransformComponent>(
                                TransformComponent(translation, rotation, scale)
                            );
                        } else if (line == "[Renderer]") {
                            std::getline(file, line);

                            Mesh *mesh = Meshes::quad;
                            std::getline(file, line);

                            Texture *texture = 0;
                            if (line != "<None>") {
                                texture = assets->load_texture(line);
                            }

                            glm::vec4 color = read_vec4(file);

                            entity.add<RendererComponent>(
                                RendererComponent(
                                    mesh,
                                    texture,
                                    color
                                )
                            );
                        }

                        std::getline(file, line);
                    }
                    std::getline(file, line);
                }
            }
        }
    }

    file.close();
}

glm::vec3 SceneFile::read_vec3(std::ifstream &inf) {
    string line;
    std::getline(inf, line);
    std::stringstream ss(line);

    f32 x, y, z;
    ss >> x >> y >> z;
    return glm::vec3(x, y, z);
}

glm::vec4 SceneFile::read_vec4(std::ifstream &inf) {
    string line;
    std::getline(inf, line);
    std::stringstream ss(line);

    f32 x, y, z, w;
    ss >> x >> y >> z >> w;
    return glm::vec4(x, y, z, w);
}

void SceneFile::write() {
    std::ofstream of(path);
    
    of << "[Entities]\n";
    for (auto kv : scene->entity_map) {
        Entity entity = kv.second;

        of << "[Entity]\n";
        of << entity.get<TagComponent>().tag << "\n";
        of << "\n";

        if (entity.has<TransformComponent>()) {
            of << "[Transform]\n";
            auto &tc = entity.get<TransformComponent>();
            
            write(of, tc.translation);
            write(of, tc.rotation);
            write(of, tc.scale);
        }

        if (entity.has<RendererComponent>()) {
            of << "[Renderer]\n";
            auto &rc = entity.get<RendererComponent>();
            
            of << "Quad\n";
            if (rc.texture) {
                of << rc.texture->path << "\n";
            } else {
                of << "<None>\n";
            }
            write(of, rc.color);
        }

        of << "[EntityEnd]\n";
    }
    of << "[EntitiesEnd]\n";

    of.close();
}

void SceneFile::write(std::ofstream &of, glm::vec3 vec) {
    of << vec.x << " " << vec.y << " " << vec.z << "\n";
}

void SceneFile::write(std::ofstream &of, glm::vec4 vec) {
    of << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "\n";
}

ProjectFile::ProjectFile(string path, Project *project)
    : path(path), project(project) {
}

void ProjectFile::read() {
    std::ifstream file(path);

    Assets *assets = project->assets;

    string line;

    while (std::getline(file, line)) {
        if (line == "[Project]") {
            std::getline(file, line);
            project->name = line;
        } else if (line == "[Textures]") {
            std::getline(file, line);

            while (line != "[TexturesEnd]") {
                assets->load_texture(line);
                std::getline(file, line);
            }
        }
    }
}

void ProjectFile::write() {
    std::ofstream of(path);

    of << "[Project]\n";
    of << project->name << "\n";

    of << "[Textures]\n";
    for (auto kv : project->assets->textures) {
        of << kv.first << "\n";
    }
    of << "[TexturesEnd]\n";
}

Project::Project(string path)
    : path(path) {
    assets = new Assets(path + "Assets/");
}

Project::Project(string path, string name)
    : path(path), name(name) {
    assets = new Assets(path + "Assets/");
}

Project::~Project() {
}

void Project::load() {
    if (!std::filesystem::is_directory(path)) {
        log_fatal("Invalid project path '%s'", path.c_str());
    }
    
    std::filesystem::create_directory(path + "Assets/");

    string project_file = path + "Project.yak";

    if (std::filesystem::is_regular_file(project_file)) {
        ProjectFile pf(project_file, this);
        pf.read();
    }

    log_info("Loaded Project '%s'", name.c_str());
}

void Project::save() {
    string project_file = path + "Project.yak";
    ProjectFile pf(project_file, this);
    pf.write();

    log_info("Saved Project '%s'", name.c_str());
}

Scene *load_scene_file(string path, Assets *assets) {
    Scene *scene = new Scene(path);
    SceneFile sf(path, scene, assets);
    sf.read();

    log_info("Loaded Scene '%s'", path.c_str());

    return scene;
}

void save_scene_file(Scene *scene, Assets *assets) {
    SceneFile sf(scene->path, scene, assets);
    sf.write();

    log_info("Saved Scene '%s'", scene->path.c_str());
}
