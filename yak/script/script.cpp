#include "script.h"

#ifndef _WIN64
#include <dlfcn.h>
#endif

Script::Script(string path)
	: path(path) {
}

void Script::reload() {
#ifndef _WIN64
	void *script_so = dlopen(path.c_str(), RTLD_NOW);

	ScriptableEntity entity;
	entity.create_function = (create_function_type) dlsym(script_so, "create");
	entity.destroy_function = (destroy_function_type) dlsym(script_so, "destroy");
	entity.update_function = (update_function_type) dlsym(script_so, "update");

	dlclose(script_so);
#endif
}
