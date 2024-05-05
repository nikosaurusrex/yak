#ifndef SCRIPT_H
#define SCRIPT_H

#include <common.h>

#include <glm/glm.hpp>

#include "entity/components.h"

typedef void (*set_function_type)(const char *tag, TransformComponent *transform, glm::vec3 *camera_pos);
typedef void (*create_function_type)();
typedef void (*destroy_function_type)();
typedef void (*update_function_type)();
typedef void (*on_key_function_type)(s32 key);

struct Script {
	void *handle = 0;
	string path;
	string compiled_path;

	bool loaded = false;

	set_function_type set_function;
	create_function_type create_function;
	destroy_function_type destroy_function;
	update_function_type update_function;
	on_key_function_type on_key_down_function;
	on_key_function_type on_key_release_function;

	Script(string path);
	~Script();
	void load();
	void recompile();
};

#endif
