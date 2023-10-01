#include "script.h"

#include <sstream>
#include <filesystem>

#ifdef _WIN64
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace fs = std::filesystem;

Script::Script(string path)
	: path(path) {

	fs::path lib_path = fs::path(path);

#ifdef _WIN64
	lib_path.replace_extension(".dll");
#else
	lib_path.replace_extension(".dylib");
#endif

	compiled_path = lib_path.string();
}

Script::~Script() {
#ifdef _WIN64
	if (handle) {
		FreeLibrary((HMODULE)handle);
	}
#else
	if (handle) {
		dlclose(handle);
	}
#endif
}

void Script::load() {
	log_info("Load Script '%s'", path.c_str());

#ifdef _WIN64
	HINSTANCE lib = LoadLibraryA(compiled_path.c_str());
	if (!lib) {
		log_error("Failed to open script");
		return;
	}

	set_function = (set_function_type) GetProcAddress(lib, "set");
	create_function = (create_function_type) GetProcAddress(lib, "create");
	destroy_function = (destroy_function_type) GetProcAddress(lib, "destroy");
	update_function = (update_function_type) GetProcAddress(lib, "update");

	handle = lib;
	loaded = true;

#else
	handle = dlopen(path.c_str(), RTLD_NOW);

	set_function = (create_function_type) dlsym(handle, "set");
	create_function = (create_function_type) dlsym(handle, "create");
	destroy_function = (destroy_function_type) dlsym(handle, "destroy");
	update_function = (update_function_type) dlsym(handle, "update");

	loaded = true;
#endif
}

void Script::recompile() {
	log_info("Recompile Script '%s'", path.c_str());

#ifdef _WIN64
	if (handle) {
		FreeLibrary((HMODULE)handle);
	}
#else
	if (handle) {
		dlclose(handle);
	}
#endif

	/* TODO: change compiler according to settings */
	const char *compiler = "clang++"; 
	std::stringstream compile_cmd;

	compile_cmd << compiler << " -shared -o "
				<< compiled_path << " " << path;

	system(compile_cmd.str().c_str());

	load();
}
