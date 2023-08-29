#include "project.h"

#include <filesystem>
#include <sstream>

#include "entity/components.h"

ProjectFile::ProjectFile(string path, Project *project)
    : path(path), project(project) {
}

void ProjectFile::read() {
    std::ifstream file(path);

    Scene *scene = project->scene;
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
        } else if (line == "[Entities]") {
            std::getline(file, line);

            while (line != "[EntitiesEnd]") {
                if (line == "[Entity]") {
                    std::getline(file, line);
                    Entity entity = scene->create_entity(line);
                    
                    std::getline(file, line);
                    while (line != "[EntityEnd]") {
                        if (line == "[Transform]") {
                            glm::vec3 translation = read_vec3(file);
                            glm::vec3 scale = read_vec3(file);

                            entity.add<TransformComponent>(
                                TransformComponent(translation, scale)
                            );
                        } else if (line == "[Renderer]") {
                            std::getline(file, line);

                            Mesh *mesh = Meshes::quad;
                            std::getline(file, line);

                            Texture *texture = assets->load_texture(line);

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

glm::vec3 ProjectFile::read_vec3(std::ifstream &inf) {
    string line;
    std::getline(inf, line);
    std::stringstream ss(line);

    f32 x, y, z;
    ss >> x >> y >> z;
    return glm::vec3(x, y, z);
}

glm::vec4 ProjectFile::read_vec4(std::ifstream &inf) {
    string line;
    std::getline(inf, line);
    std::stringstream ss(line);

    f32 x, y, z, w;
    ss >> x >> y >> z >> w;
    return glm::vec4(x, y, z, w);
}

void ProjectFile::write() {
    std::ofstream of(path);

    Scene *scene = project->scene;
    Assets *assets = project->assets;

    of << "[Project]\n";
    of << project->name << "\n";

    of << "[Textures]\n";
    for (auto kv : assets->textures) {
        of << kv.first << "\n";
    }
    
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
            write(of, tc.scale);
        }

        if (entity.has<RendererComponent>()) {
            of << "[Renderer]\n";
            auto &rc = entity.get<RendererComponent>();
            
            of << "Quad\n";
            of << rc.texture->path << "\n";
            write(of, rc.color);
        }

        of << "[EntityEnd]\n";
    }
    of << "[EntitiesEnd]\n";

    of.close();
}

void ProjectFile::write(std::ofstream &of, glm::vec3 vec) {
    of << vec.x << " " << vec.y << " " << vec.z << "\n";
}

void ProjectFile::write(std::ofstream &of, glm::vec4 vec) {
    of << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "\n";
}

Project::Project(string path)
    : path(path) {
    scene = new Scene();
    assets = new Assets(path + "assets/");
}

Project::Project(string path, string name)
    : path(path), name(name) {
    scene = new Scene();
    assets = new Assets(path + "assets/");
}

Project::~Project() {
}

void Project::load() {
    if (!std::filesystem::is_directory(path)) {
        log_fatal("Invalid project path '%s'", path.c_str());
    }
    
    std::filesystem::create_directory(path + "assets/");

    string project_file = path + "project.yak";

    if (std::filesystem::is_regular_file(project_file)) {
        ProjectFile pf(project_file, this);
        pf.read();
    }

    log_info("Loaded Project '%s'", name.c_str());
}

void Project::save() {
    string project_file = path + "project.yak";
    ProjectFile pf(project_file, this);
    pf.write();

    log_info("Saved Project '%s'", name.c_str());
}
