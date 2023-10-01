#ifndef SCRIPT_H
#define SCRIPT_H

#include <common.h>

#include <glm/glm.hpp>

typedef void (*set_function_type)(glm::vec3* position, bool *pressed);
typedef void (*create_function_type)();
typedef void (*destroy_function_type)();
typedef void (*update_function_type)();

struct Script {
	void *handle = 0;
	string path;
	string compiled_path;

	bool loaded = false;

	set_function_type set_function;
	create_function_type create_function;
	destroy_function_type destroy_function;
	update_function_type update_function;

	Script(string path);
	~Script();
	void load();
	void recompile();
};

#endif
