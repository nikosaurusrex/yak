#include "project.h"

#include <filesystem>
#include <sstream>

#include "entity/components.h"

template <typename T>
struct ConfigValueEncoder {
};

template<>
struct ConfigValueEncoder<string> {
	string encode(string value) {
		return value;
	}

	string decode(string value) {
		return value;
	}
};

template<>
struct ConfigValueEncoder<bool> {
	string encode(bool value) {
		return value ? "true" : "false";
	}

	bool decode(string value) {
		return value == "true" ? true : false;
	}
};

template<>
struct ConfigValueEncoder<const char *> {
	string encode(const char *value) {
		return string(value);
	}

	const char *decode(string value) {
		const char *cstr = value.c_str();
		return strdup(cstr);
	}
};

template<size_t n>
struct ConfigValueEncoder<char[n]> {
	string encode(const char *value) {
		return string(value);
	}
};

template<>
struct ConfigValueEncoder<s32> {
	string encode(s32 value) {
		std::stringstream stream;

		stream << value;

		return stream.str();
	}

	s32 decode(string value) {
		return std::stoi(value);
	}
};

template<>
struct ConfigValueEncoder<f32> {
	string encode(f32 value) {
		std::stringstream stream;

		stream << value;

		return stream.str();
	}

	f32 decode(string value) {
		return std::stof(value);
	}
};

template<>
struct ConfigValueEncoder<glm::vec3> {
	string encode(glm::vec3 value) {
		std::stringstream stream;

		stream << value.x << " " << value.y << " " << value.z;

		return stream.str();
	}

	glm::vec3 decode(string value) {
		std::stringstream stream(value);

		glm::vec3 vec;
		stream >> vec.x;	
		stream >> vec.y;	
		stream >> vec.z;	

		return vec;
	}
};

template<>
struct ConfigValueEncoder<glm::vec4> {
	string encode(glm::vec4 value) {
		std::stringstream stream;

		stream << value.x << " " << value.y << " " << value.z << " " << value.w;

		return stream.str();
	}

	glm::vec4 decode(string value) {
		std::stringstream stream(value);

		glm::vec4 vec;
		stream >> vec.x;	
		stream >> vec.y;	
		stream >> vec.z;	
		stream >> vec.w;	

		return vec;
	}
};

struct ConfigValue {
	string data;

	template <typename T>
	ConfigValue &operator=(const T &value) {
		ConfigValueEncoder<T> encoder;
		data = encoder.encode(value);
		return *this;
	}

	template <typename T>
	T as() {
		ConfigValueEncoder<T> encoder;
		return encoder.decode(data);
	}
};

struct ConfigSection {
	map<string, ConfigValue> values;

	ConfigValue &operator[](string name) {
		return values[name];
	}
};

struct Config {
	string path;
	map<string, ConfigSection> sections;

	Config(string path) : path(path) {
	}

	ConfigSection &operator[](string name) {
		return sections[name];
	}

	bool has(string name) {
		return sections.find(name) != sections.end();
	}

	bool read() {
		FILE *file = fopen(path.c_str(), "r");
		if (!file) {
			return false;
		}

		const u32 max_buffer_size = 512;
		char line_buffer[max_buffer_size];
		char buffer[max_buffer_size];

		ConfigSection *current_section;

		while (!feof(file)) {
			fgets(line_buffer, max_buffer_size, file);

			if (line_buffer[0] == '\n') {
				continue;
			} else if (line_buffer[0] == '[') {
				s32 start = 1;
				s32 pos = 0;
				s32 line_buffer_length = strlen(line_buffer);

				while (pos < line_buffer_length && line_buffer[pos] != ']') {
					pos++;
				}

				if (pos == line_buffer_length) {
					assert(0 && "Didn't find closing ']'");
					return false;
				}

				memcpy(buffer, line_buffer + start, pos - start);
				buffer[pos - 1] = 0;

				string name = string(buffer);

				current_section = &sections[name];
			} else {
				s32 start = 0;
				s32 pos = 0;
				s32 line_buffer_length = strlen(line_buffer);

				while (pos < line_buffer_length && line_buffer[pos] != '=') {
					pos++;
				}

				if (pos == line_buffer_length) {
					assert(0 && "Didn't find '='");
					return false;
				}

				memcpy(buffer, line_buffer + start, pos - start);
				buffer[pos] = 0;

				string name = string(buffer);

				pos++; // skip '='
				start = pos;
				
				while (pos < line_buffer_length && line_buffer[pos] != '\n') {
					pos++;
				}

				if (pos == line_buffer_length) {
					assert(0 && "Didn't find new line '\n'");
					return false;
				}

				memcpy(buffer, line_buffer + start, pos - start);
				buffer[pos - start] = 0;

				string value = string(buffer);
			
				(*current_section)[name] = value;
			}
		}

		fclose(file);
		return true;
	}

	bool write() {
		FILE *file = fopen(path.c_str(), "w");
		if (!file) {
			return false;
		}

		for (auto &section : sections) {
			fprintf(file, "[%s]\n", section.first.c_str());
			
			for (auto &value : section.second.values) {
				fprintf(file, "%s=%s\n", value.first.c_str(), value.second.data.c_str());
			}

			putc('\n', file);
		}

		fclose(file);
		return true;
	}
};

Project::Project(string path)
    : path(path) {
    assets = new Assets(path);
}

Project::Project(string path, string name)
    : path(path), name(name) {
    assets = new Assets(path);
}

Project::~Project() {
}

void Project::load() {
    if (!std::filesystem::is_directory(path)) {
        log_fatal("Invalid project path '%s'", path.c_str());
    }

    string project_file = path + "Project.yak";

    if (std::filesystem::is_regular_file(project_file)) {
		Config config(project_file);
		config.read();

		name = config["Project"]["Name"].as<string>();
    }

    log_info("Loaded Project '%s'", name.c_str());
}

void Project::save() {
    string project_file = path + "Project.yak";

	Config config(project_file);
	config["Project"]["Name"] = name;
	config.write();

    log_info("Saved Project '%s'", name.c_str());
}

Scene *load_scene_file(string path, Assets *assets) {
    Scene *scene = new Scene(path);

	Config config(path);
	config.read();
	
	s32 entity_count = config["Entities"]["Count"].as<s32>();

	for (s32 i = 0; i < entity_count; ++i) {
		string name = "Entity" + std::to_string(i + 1);

		string tag = config[name]["Tag"].as<string>();
		Entity entity = scene->create_entity(tag);

		string transform_id = name + "_Transform";
		if (config.has(transform_id)) {
			auto &transform = config[transform_id];
			auto translation = transform["Translation"].as<glm::vec3>();
			auto rotation = transform["Rotation"].as<glm::vec3>();
			auto scale = transform["Scale"].as<glm::vec3>();

			entity.add<TransformComponent>(
				TransformComponent(translation, rotation, scale)
			);
		}

		string renderer_id = name + "_Renderer";
		if (config.has(renderer_id)) {
			auto &renderer = config[renderer_id];

			Mesh *mesh = Meshes::quad;

			string texture_path = renderer["Texture"].as<string>();
			Texture *texture = 0;
			if (texture_path != "<None>") {
				texture = assets->load_texture(texture_path);
			}

			auto color = renderer["Color"].as<glm::vec4>();

			entity.add<RendererComponent>(
				RendererComponent(
					mesh,
					texture,
					color
				)
			);
		}

		string camera_id = name + "_Camera";
		if (config.has(camera_id)) {
			auto &camera = config[camera_id];

			auto position = camera["Position"].as<glm::vec3>();
			auto rotation = camera["Rotation"].as<f32>();
			auto is_main_camera = camera["IsMain"].as<bool>();

			entity.add<CameraComponent>(CameraComponent());

			auto &component = entity.get<CameraComponent>();
			component.position = position;
			component.rotation = rotation;
			component.is_main_camera = is_main_camera;
		}

		string script_id = name + "_Script";
		if (config.has(script_id)) {
			auto &script_component = config[script_id];

			auto path = script_component["Path"].as<string>();

			Script *script = 0;
			if (path != "<None>") {
				script = assets->load_script(path);
			}

			entity.add<ScriptComponent>(ScriptComponent(script));
		}
	}

    log_info("Loaded Scene '%s'", path.c_str());

    return scene;
}

void save_scene_file(Scene *scene, Assets *assets) {
	Config config(scene->path);

	s32 entity_count = scene->entity_map.size();
	config["Entities"]["Count"] = entity_count;

	s32 i = 1;
	for (auto &entities : scene->entity_map) {
		auto &entity = entities.second;

		auto name = "Entity" + std::to_string(i);
		config[name]["Tag"] = entity.get<TagComponent>().tag;

		if (entity.has<TransformComponent>()) {
			auto &tc = entity.get<TransformComponent>();
			auto &transform = config[name + "_Transform"];

			transform["Translation"] = tc.translation;
			transform["Rotation"] = tc.rotation;
			transform["Scale"] = tc.scale;
		}

		if (entity.has<RendererComponent>()) {
			auto &rc = entity.get<RendererComponent>();
			auto &renderer = config[name + "_Renderer"];

			renderer["Mesh"] = "Quad";

            if (rc.texture) {
				renderer["Texture"] = rc.texture->path;
            } else {
				renderer["Texture"] = "<None>";
            }

			renderer["Color"] = rc.color;
		}

		if (entity.has<CameraComponent>()) {
			auto &cc = entity.get<CameraComponent>();
			auto &camera = config[name + "_Camera"];

			camera["Position"] = cc.position;
			camera["Rotation"] = cc.rotation;
			camera["IsMain"] = cc.is_main_camera;
		}

		if (entity.has<ScriptComponent>()) {
			auto &cc = entity.get<ScriptComponent>();
			auto &camera = config[name + "_Script"];

			camera["Path"] = cc.script->path;
		}
		i++;
	}

	config.write();
    log_info("Saved Scene '%s'", scene->path.c_str());
}
