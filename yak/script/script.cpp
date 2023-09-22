#include "script.h"

#include <dlfcn.h>

Script::Script(string path)
	: path(path) {
}

void Script::reload() {
	void *script_so = dlopen(path.c_str(), RTLD_NOW);

	ScriptableEntity entity;
	entity.create_function = (create_function_type) dlsym(script_so, "create");
	entity.destroy_function = (destroy_function_type) dlsym(script_so, "destroy");
	entity.update_function = (update_function_type) dlsym(script_so, "update");

	dlclose(script_so);
}
