#ifndef SCRIPT_H
#define SCRIPT_H

#include <yakpch.h>

typedef void (*create_function_type)();
typedef void (*destroy_function_type)();
typedef void (*update_function_type)();

struct ScriptableEntity {
	create_function_type create_function;
	destroy_function_type destroy_function;
	update_function_type update_function;
};

struct Script {
	string path;

	Script(string path);
	void reload();
};

#endif
