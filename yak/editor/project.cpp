#include "project.h"

#include <filesystem>

#include <inicpp.h>

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

void Project::load() {
    if (!std::filesystem::is_directory(path)) {
        log_fatal("Invalid project path '%s'", path.c_str());
    }
    
    std::filesystem::create_directory(path + "assets/");

    string project_file = path + "project.yak";

    if (std::filesystem::is_regular_file(project_file)) {
        load_project_file(project_file);
    } else {
        create_project_file(project_file);
    }

    log_info("Loaded Project '%s'", name.c_str());
}

void Project::load_project_file(string path) {
    ini::IniFile ini;
    ini.load(path);

    name = ini["Project"]["Name"].as<string>(); 

    auto textures = ini["Textures"];
    for (auto kv : textures) {
        string texture = textures[kv.first].as<string>(); 
        assets->load_texture(texture);
    }
}

void Project::create_project_file(string path) {
    ini::IniFile ini;

    ini["Project"]["Name"] = name;

    ini.save(path);
}

Project::~Project() {
}
